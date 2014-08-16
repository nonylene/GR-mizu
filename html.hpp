#include <rxduino.h>
#include <ethernet.h>

void returnHTML(EthernetClient& client,char value[]){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type:text/html; charset=UTF-8");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title>GR-SAKURA</title>");
    client.println("多重アクセスで落ちるので丁寧に扱ってください<br/>");
    client.println("message:");
    client.println( value );
    client.println("<form method=GET>");
    client.println("<input type=submit name=m value=GO /><br />");
    client.println("秒数: <input type=number name=u /><br />");
    client.println("-----manage-----<br/>");
    client.println("<form method=GET>");
    client.println("<input type=submit name=m value=UP />");
    client.println("<form method=GET>");
    client.println("<input type=submit name=m value=DOWN />");
    client.println("<form method=GET>");
    client.println("<input type=submit name=m value=STOP /><br />");
    client.println("</form>");
    client.println("</body>");
    client.println("</html>");
}

