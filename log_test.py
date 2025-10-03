import websocket

ws = websocket.WebSocket()
ws.connect("ws://localhost:9002")
ws.send("SUBSCRIBE:log")

while True:
    msg = ws.recv()
    print("Cliente LOG recibió:", msg)
