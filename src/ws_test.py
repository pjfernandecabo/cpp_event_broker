import websocket

def on_message(ws, message):
    print("Recibido:", message)

def on_close(ws, close_status_code, close_msg):
    print("Conexión cerrada")

ws = websocket.WebSocketApp("ws://localhost:9002",
                            on_message=on_message,
                            on_close=on_close)
ws.run_forever()