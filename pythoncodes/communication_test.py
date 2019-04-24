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
slack_bot_token = "xoxb-170654737781-602026864064-nRWcflzpTNnZs2SLToUuPEWG"
slack_signing_secret = "09e62ca3df5b1fea582cda5aff964eb2"

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
    # If the incoming message contains "hi", then respond with a "Hello" message
    print(message.get('text'))
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

    return make_response("", 200)

# Example responder to greetings
@app.route("/slack/events", methods = ["GET"])
def index():
    slack_client.api_call(
        "chat.postMessage",
        channel="#test",
        text="You have got an order !",
        attachments=ordering_message_json
    )
    return make_response("", 200)

#redirect from button
@app.route("/slack/json_html", methods = ["POST"])
def json_html():

    # Parse the request payload
    form_json = json.loads(request.form["payload"])
    timeStamp = form_json["original_message"]["ts"]
    slackChannel = form_json["channel"]["id"]
    val = form_json["actions"][0]["value"]

    if val == "done":
        response = slack_client.api_call(
            "chat.update",
            ts = timeStamp,
            channel=slackChannel,
            text = "",
            attachments = reply_ordered_message
        )

        slack_client.api_call(
            "chat.postMessage",
            channel=slackChannel,
            text="",
            attachments=received_message_json
        )
    else:
        response = slack_client.api_call(
            "chat.update",
            ts = timeStamp,
            channel=slackChannel,
            text = "",
            attachments = reply_received_message
        )

    return make_response("", 200)

# send UDP Signal
def transmit(data):
    UDP_IP = "127.0.0.1"
    UDP_PORT = 6666
    sock = socket.socket(socket.AF_INET,  # Internet
                         socket.SOCK_DGRAM)  # UDP
    sock.sendto(data, (UDP_IP, UDP_PORT))

# your attachment
attachments_json = [
    {
        "fallback": "Upgrade your Slack client to use messages like these.",
        "color": "#258ab5",
        "attachment_type": "default",
        "callback_id": "the_greatest_war",
        "actions": [
            {
                "name": "choco1",
                "text": "kinoko",
                "value": "kinoko",
                "type": "button"
            },
            {
                "name": "choco2",
                "text": "takenoko",
                "value": "takenoko",
                "type": "button"
            }
        ]
    }
]

reply_ordered_message = [
    {
        "title" : "Thank you for ordering !",
        # "text" : "when you receive package, please put correct place",
        "fallback" : "bbb",
        "color": "#3AA3E3",
        "attachment_type": "default",
        "callback_id": "reply",
        "actions" : []
    }
]

reply_received_message = [
    {
        "title" : "Thank you for receiving package !",
        "text" : "This ordering have finished.:tada:",
        "fallback" : "bbb",
        "color": "#DC143C",
        "attachment_type": "default",
        "callback_id": "reply",
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
]

ordering_message_json = {
    "title" : "New Order Aleart !",
    "text" : "Please finish ordering using the URL below !!!! \n kzkyuta.net",
    "fallback": "aaa",
    "color": "#3AA3E3",
    "attachment_type": "default",
    "callback_id": "done",
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

received_message_json = [
    {
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
]

# thread for udp server. builed the class and call it using curl. I do not know why I need to call like this.
class ServerThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.kill_flag = False
        self.udpServSock = socket(AF_INET, SOCK_DGRAM)
        self.udpServSock.bind(("127.0.0.1", 5824))
        self.BUFSIZE = 1024
        self.data = "a"
        print(self.data)

    def run(self):
        while True:
            try:
                data, addr = self.udpServSock.recvfrom(self.BUFSIZE)
                recv_msg = json.loads(data)

                ordering_message_json["text"] = "Please finish ordering " + recv_msg["name"] + " from " + recv_msg["pageName"] + "\n" +recv_msg["URL"]

                jsonOut = []
                jsonOut.append(ordering_message_json)

                slack_client.api_call(
                    "chat.postMessage",
                    channel="#test",
                    text="You have got an order !",
                    attachments=jsonOut
                )
            except:
                pass

if __name__ == '__main__':
    app.debug = True
    app.run(port=3000)

# Once we have our event listeners configured, we can start the
# Flask server with the default `/events` endpoint on port 3000
# slack_events_adapter.start(port=3000)
