https://developer.mozilla.org/pt-BR/docs/Web/HTTP/Controle_Acesso_CORS
https://github.com/me-no-dev/ESPAsyncWebServer#headers
192.168.1.209
http://sp.contahub.com/#vendas
http://sp.contahub.com/#config.var#_tab=Local
https://arduinojson.org/v6/example/

JSON:
return "{"
                  + " \"resultado\":" + JsonCodec.toJson( this.resultado )
                  + ",\"buffer\":" + JsonCodec.toJson( buffer )
                  + ",\"gramas\":" + JsonCodec.toJson( gramas )
                  + ",\"precoKg\":" + JsonCodec.toJson( precoKg )
                  + ",\"total\":" + JsonCodec.toJson( total )
                  + "}"
                  ;

response.setHeader(HEADER.ACCESS_CONTROL_ALLOW_ORIGIN, referrer);
         response.setHeader(HEADER.ACCESS_CONTROL_ALLOW_CREDENTIALS, "true"); 
         response.setHeader(HEADER.ACCESS_CONTROL_ALLOW_METHODS, "GET,HEAD,OPTIONS,POST,DELETE,PUT");
         response.setHeader(HEADER.ACCESS_CONTROL_ALLOW_HEADERS, "Access-Control-Allow-Headers, Origin,Accept, X-Requested-With, Content-Type, Pragma, Cache-Control, Access-Control-Request-Method, Access-Control-Request-Headers");