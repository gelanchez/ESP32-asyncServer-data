#!/usr/bin/python3 -O
# -*- coding: utf-8 -*-

# GPL-3.0. José Ángel Sánchez (https://github.com/gelanchez)

"""Simple websocket client.

Usage:
    python3 websocketclient.py
"""

import websocket

try:
    import thread
except ImportError:
    import _thread as thread
import time

def on_message(ws, message):
    print(message)

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://10.42.0.190/ws",
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.run_forever()