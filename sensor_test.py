import websocket

ws = websocket.WebSocket()
ws.connect("ws://localhost:9002")
ws.send("SUBSCRIBE:sensor")

while True:
    msg = ws.recv()
    print("Cliente SENSOR recibió:", msg)
