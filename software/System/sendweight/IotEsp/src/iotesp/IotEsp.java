package iotesp;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class IotEsp {

    public static void main(String[] args) throws IOException {

        try(ServerSocket listener = new ServerSocket(8090)) {
            while(true){
                Socket socket = listener.accept();
                socket.setKeepAlive(true);
                System.out.println("Client Connected");
                try{
                    BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    System.out.println("Client response: " + in.readLine());

                    BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
                    System.out.println("Sending Message...");
                    out.write("Comunicação feita com sucesso");
                    out.flush();
                } finally {
                    socket.close();
                }
            }
        }
    } 
}
