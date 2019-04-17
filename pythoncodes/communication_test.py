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

# Example responder to greetings
@app.route("/slack/events", methods = ["GET"])
def index():
    print("OK")
    slack_client.api_call(
        "chat.postMessage",
        channel="#test",
        text="Which do you like ?",
        attachments=attachments_json
    )
    return make_response("", 200)

#redirect from button
@app.route("/slack/json_html", methods = ["POST"])
def json_html():

    # Parse the request payload
    form_json = json.loads(request.form["payload"])

    val = form_json["actions"][0]["value"]
    if val == "kinoko":
        response_text = "battle"
    else:
        response_text = "friend"
    response = slack_client.api_call(
        "chat.postMessage",
        channel="#test",
        text=response_text,
        attachments=[]
    )
    return make_response("", 200)

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

if __name__ == '__main__':
    app.debug = True
    app.run(port=3000)
# Once we have our event listeners configured, we can start the
# Flask server with the default `/events` endpoint on port 3000
# slack_events_adapter.start(port=3000)
