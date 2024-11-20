import socket
from time import sleep


def connect_to_server(host, port):
    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        client_socket.connect((host, port))
        print(f"Connected to {host}:{port}")

        # Send a message to the server
        message = "Hello, Server!"
        client_socket.sendall(message.encode("utf-8"))
        print(f"Sent: {message}")

        # Receive a response from the server
        response = client_socket.recv(1024)
        print(f"Received: {response.decode('utf-8')}")

        sleep(10000)
    except socket.error as e:
        print(f"Socket error: {e}")
    finally:
        # Close the connection
        client_socket.close()
        print("Connection closed")


if __name__ == "__main__":
    host = "127.0.0.1"  # Server address
    port = 8080  # Server port
    connect_to_server(host, port)
