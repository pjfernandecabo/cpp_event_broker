import asyncio
import websockets

# ======================================================
# Cliente WebSocket que se conecta al broker C++
# Recibe eventos y puede enviar un mensaje de 'UNSUBSCRIBE'
# ======================================================

async def client_behavior():
    uri = "ws://localhost:9002"

    async with websockets.connect(uri) as websocket:
        print("[Python] Conectado al servidor WebSocket")

        # Simulamos recibir algunos mensajes
        try:
            for i in range(3):
                message = await asyncio.wait_for(websocket.recv(), timeout=5)
                print(f"[Python] Recibido: {message}")
        except asyncio.TimeoutError:
            print("[Python] No llegaron más mensajes en el tiempo esperado.")

        # Enviamos una orden de desuscripción al servidor
        unsubscribe_msg = "UNSUBSCRIBE"
        await websocket.send(unsubscribe_msg)
        print(f"[Python] Enviado mensaje de desuscripción: {unsubscribe_msg}")

        # Esperamos confirmación o cierre
        try:
            response = await asyncio.wait_for(websocket.recv(), timeout=3)
            print(f"[Python] Respuesta del servidor: {response}")
        except asyncio.TimeoutError:
            print("[Python] Sin respuesta del servidor tras desuscribirse.")

        # Cerramos conexión
        await websocket.close()
        print("[Python] Conexión cerrada correctamente.")

# ======================================================
# Punto de entrada principal
# ======================================================
if __name__ == "__main__":
    asyncio.run(client_behavior())

