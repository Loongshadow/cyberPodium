//提供websocket连接服务支持
package com.example.cyberpodium_android;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;
import org.json.JSONObject;

import java.net.URI;
import java.net.URISyntaxException;

public class MyWebSocket extends WebSocketClient {

    public MyWebSocket(String serverUrl) throws URISyntaxException {
        super(new URI(serverUrl));
    }

    @Override
    public void onOpen(ServerHandshake handshake) {
        //  WebSocket连接已成功建立
    }

    @Override
    public void onMessage(String message) {
    }

    @Override
    public void onClose(int code, String reason, boolean remote) {
        //  WebSocket连接已关闭
    }

    @Override
    public void onError(Exception ex) {
    }
}
