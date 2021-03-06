# This app is to manage consumable items at UTAIL.
# This app was comvided with Qt.
# interactive button example
    # https://qiita.com/sh-tatsuno/items/55cd5f9e78b212fb57c2
# basici slack api usage
    # https://github.com/slackapi/python-slack-events-api/tree/master/example
# to make local host public, there is a app to build https.
    # https://stackoverflow.com/questions/38050182/how-to-generate-fixed-url-with-ngrok

# from slackeventsapi import SlackEventAdapter
from flask import Flask, request, make_response, Response
from slackeventsapi import SlackEventAdapter
from slackclient import SlackClient
import os
import json
from socket import *
import threading

# token from slack api manager page
slack_bot_token = os.environ["SLACK_BOT_TOKEN"] # export SLACK_BOT_TOKEN=xxxXXxxXXxXXxXXXXxxxX.xXxxxXxxxx
slack_signing_secret = os.environ["SLACK_SIGNING_SECRET"] # export SLACK_SIGNING_SECRET=xxxxxxxxXxxXxxXxXXXxxXxxx
# Build slack client with slack bot token from oath page
slack_client = SlackClient(slack_bot_token)

# make flask sever
app = Flask(__name__)

# this one is needed to confirm the connection between this code and slack
slack_events_adapter = SlackEventAdapter(slack_signing_secret, "/confirm", app)

# this code confirms the connection between slack and this python code
@slack_events_adapter.on("message")
def handle_message(event_data):
    message = event_data["event"]
    if message.get("subtype") is None and "hi" in message.get('text'):
        channel = message["channel"]
        message = "Hello <@%s>! :tada:" % message["user"]
        slack_client.api_call("chat.postMessage", channel=channel, text=message)

# the thread for udp receiving, it is needed to call from Flask. which means call using "curl https://******.ngrok.io/python/thread_start"
@app.route("/python/thread_start", methods = ["GET"])
def thread_start():
    th = ServerThread()
    th.setDaemon(True)
    th.start()
    print("server started")
    return make_response("", 200)

#redirect from button
@app.route("/slack/json_html", methods = ["POST"]) # For interactive Components
def json_html():

    # Parse the request payload
    form_json = json.loads(request.form["payload"])
    timeStamp = form_json["original_message"]["ts"]
    slackChannel = form_json["channel"]["id"]
    val = form_json["actions"][0]["value"]
    name = form_json["actions"][0]["name"]
    fallback = form_json["original_message"]["attachments"][0]["fallback"]
    callback_id = form_json["original_message"]["attachments"][0]["callback_id"]

    reply_message = {}
    reply_message["callback_id"] = callback_id  # id
    reply_message["val"] = val  # done, received;
    reply_message["name"] = name  # name
    reply_message["pageName"] = fallback  # pagename
    reply_message["timeStamp"] = timeStamp  # timeStamp
    reply_message["slackChannel"] = slackChannel  #slackChannel

    # when the "done" btn was pushed on slack interface
    if val == "done":
        print("got ordered report from slack")
        transmit(json.dumps(reply_message))

    else:
        print("got received report from slack")
        transmit(json.dumps(reply_message))
    return make_response("", 200)

# send UDP Signal
def transmit(data):
    UDP_IP = "127.0.0.1"
    UDP_PORT = 5826
    sock = socket(AF_INET,  # Internet
                         SOCK_DGRAM)  # UDP
    sock.sendto(data, (UDP_IP, UDP_PORT))

# reply messag after pushing "done" btn
reply_ordered_message = [{
    "title" : "Thank you for ordering !",
    "fallback" : "bbb",
    "color": "#3AA3E3",
    "attachment_type": "default",
    "callback_id": "reply",
    "actions" : []
}]

reply_received_message = [
    {
        "title" : "Thank you for receiving package !",
        "text" : "This ordering have finished.:tada:",
        "fallback" : "bbb",
        "color": "#DC143C",
        "attachment_type": "default",
        "callback_id": "reply",
        "actions" : []
    }
]

ordering_message_json = {
    "title" : "New Order Aleart !",
    "text" : "Please finish ordering using the URL below !!!! \n kzkyuta.net",
    "fallback": "bbb",
    "color": "#3AA3E3",
    "attachment_type": "default",
    "callback_id": "done",
    "image_url": "https://github.com/kzkyuta/ConsumablesManager/blob/feature/UDP_connection_btw_python_and_Qt/DataBase/img/Curing_tape.jpg?raw=true",
    "actions" : [
        {
            "name" : "done",
            "text" : "Done",
            "type" : "button",
            "value" : "done",
            "style": "primary",
            "confirm" : {
                "title" : "Are you sure ?",
                "text" : "Have you ordered ?",
                "ok_text" : "Yes",
                "dismiss_text" : "No"
            }
        }
    ]
}

received_message_json = {
    "title" : "Once you have received package, Finish this ordering !",
    "text" : "when you receive and put it back to correct place, please push the button and finish this ordering.",
    "fallback": "aaa",
    "color": "#DC143C",
    "attachment_type": "default",
    "callback_id": "received",
    "actions" : [
        {
            "name" : "received",
            "text" : "Received",
            "type" : "button",
            "value" : "received",
            "style": "primary",
            "confirm" : {
                "title" : "Are you sure ?",
                "text" : "Have you received package ?",
                "ok_text" : "Yes",
                "dismiss_text" : "No"
            }
        }
    ]
}

# thread for udp server. builed the class and call it using curl. I do not know why I need to call like this.
class ServerThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.kill_flag = False
        self.udpServSock = socket(AF_INET, SOCK_DGRAM)
        self.udpServSock.bind(("127.0.0.1", 5824))
        self.BUFSIZE = 1024
        self.data = "a"

    def run(self):
        while True:
            try:
                data, addr = self.udpServSock.recvfrom(self.BUFSIZE)
                recv_msg = json.loads(data)

                val = recv_msg["val"]
                id = recv_msg["id"]
                name = recv_msg["name"]
                pageName = recv_msg["pageName"]
                timeStamp = recv_msg["timeStamp"]
                slackChannel = recv_msg["slackChannel"]

                if val == "ordered":
                    print("got order request")
                    ordering_message_json["text"] = "Please finish ordering below\n" + recv_msg["name"] + " from " + recv_msg["pageName"] + " Category\n" +recv_msg["URL"]
                    ordering_message_json["fallback"] = recv_msg["pageName"]
                    ordering_message_json["callback_id"] = recv_msg["id"]
                    ordering_message_json["actions"][0]["name"] = recv_msg["name"]
                    ordering_message_json["image_url"] = "https://github.com/kzkyuta/ConsumablesManager/blob/feature/UDP_connection_btw_python_and_Qt/DataBase/img/" + recv_msg["name"] + ".jpg?raw=true"

                    jsonOut_ordered = []
                    jsonOut_ordered.append(ordering_message_json)

                    slack_client.api_call(
                        "chat.postMessage",
                        channel="#z_lab_hack",
                        # channel="#ai_item",
                        text="You have got an order !",
                        attachments=jsonOut_ordered
                    )

                    reply_message = {}
                    reply_message["val"] = val
                    reply_message["name"] = name
                    reply_message["pageName"] = pageName
                    reply_message["callback_id"] = str(id)
                    reply_message["slackChannel"] = slackChannel
                    reply_message["timeStamp"] = timeStamp
                    self.transmitData(json.dumps(reply_message))

                elif val == "done":
                    print("got ordered report from Qt")
                    response = slack_client.api_call(
                        "chat.update",
                        ts = timeStamp,
                        channel=slackChannel,
                        text = "",
                        attachments = reply_ordered_message
                    )

                    received_message_json["fallback"] = pageName
                    received_message_json["actions"][0]["name"] = name
                    received_message_json["text"] = "when you receive " + name + " and put it back to correct place, please push the button and finish this ordering."
                    received_message_json["callback_id"] = id
                    received_message_json["image_url"] = "https://github.com/kzkyuta/ConsumablesManager/blob/feature/UDP_connection_btw_python_and_Qt/DataBase/img/" + name + ".jpg?raw=true"

                    jsonOut = []
                    jsonOut.append(received_message_json)

                    slack_client.api_call(
                        "chat.postMessage",
                        channel=slackChannel,
                        text="",
                        attachments=jsonOut
                    )

                elif val == "received":
                    print("got received report from Qt")
                    response = slack_client.api_call(
                        "chat.update",
                        ts = timeStamp,
                        channel=slackChannel,
                        text = "",
                        attachments = reply_received_message
                    )

            except:
                pass

    def transmitData(self, data):

        UDP_IP = "127.0.0.1"
        UDP_PORT = 5826
        # sock = socket(socket.AF_INET,  # Internet
        #                      socket.SOCK_DGRAM)  # UDP
        sock = socket(AF_INET,  # Internet
                             SOCK_DGRAM)  # UDP
        sock.sendto(data, (UDP_IP, UDP_PORT))
        print("UDP sent")

if __name__ == '__main__':
    app.debug = True
    app.run(port=3000)

# Once we have our event listeners configured, we can start the
# Flask server with the default `/events` endpoint on port 3000
# slack_events_adapter.start(port=3000)
