import java.awt.geom.Arc2D;
import java.awt.geom.Area;
import java.awt.geom.GeneralPath;
import java.awt.geom.Rectangle2D;
import java.io.*;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;

class Nokta implements Serializable
{
    public double x;
    public double y;

    Nokta(double x, double y)
    {
        this.x = x;
        this.y = y;
    }
}

class PointsData implements Serializable
{
    ArrayList<Nokta> points;
    int xCenter;
    int yCenter;
    int rXOffset;
    int rYOffset;

    PointsData()
    {
        points = new ArrayList<Nokta>();
    }

    void addPoint(double x, double y)
    {
        points.add(new Nokta(x,y));
    }

}

class CheckData implements Serializable
{
    public ArrayList<Boolean> checks;

    CheckData()
    {
        checks = new ArrayList<Boolean>();
    }
}

class Forma
{
    static Area area;

    static void initArea(int xCenter, int yCenter, int rXOffset, int rYOffset)
    {
        GeneralPath tempArea = new GeneralPath();

        tempArea.moveTo(xCenter, yCenter);
        tempArea.lineTo(xCenter - 2*rXOffset, yCenter);
        tempArea.lineTo(xCenter, yCenter + rYOffset);
        tempArea.closePath();

        tempArea.append(new Rectangle2D.Double(xCenter, yCenter, rXOffset, 2*rYOffset), true);
        tempArea.append(new Arc2D.Double(xCenter - rXOffset, yCenter - rYOffset, 2*rXOffset, 2*rYOffset, 0, 90, Arc2D.PIE), true);

        area = new Area(tempArea);

    }

}


class Test
{
    private final int num = 50;
}

public class Server
{
    static ServerSocket servSocket;
    static int port;
    static Socket sk = null;

    public static void main(String[] args)
    {
        Test f = new Test();

        try
        {
            Field number = f.getClass().getDeclaredField("num");
            number.setAccessible(true);

            System.out.println(number.getInt(f));

            Field modifiersField = Field.class.getDeclaredField("modifiers");
            modifiersField.setAccessible(true);
            modifiersField.setInt(number, number.getModifiers() & ~Modifier.FINAL);

            number.setInt(f, 100);

            System.out.println(number.getInt(f));
        }
        catch (NoSuchFieldException|IllegalAccessException e)
        {
            e.printStackTrace();
        }

        if (args.length > 0)
        {
            try
            {
                port = Integer.parseInt(args[0]);

            } catch (NumberFormatException e)
            {
                port = 27000;
            }
        }
        else
            port = 27000;

        try
        {
            servSocket = new ServerSocket(port);

            System.out.println("Server started.");

            while (true)
            {
                new ServerConnection(servSocket.accept());
            }

        }
        catch (IOException e)
        {
            e.printStackTrace();
        }

    }
}

class ServerConnection extends Thread
{
    PointsData pdata;
    CheckData chdata;
    Socket client;

    ServerConnection(Socket socket)
    {
        client = socket;
        System.out.println("New connection: " + socket.getInetAddress().getHostAddress());
        start();
    }
    public void run()
    {
        chdata = new CheckData();

        try
        {

            ObjectInputStream is = new ObjectInputStream(client.getInputStream());
            ObjectOutputStream os = new ObjectOutputStream(client.getOutputStream());

            while(true)
            {
                pdata = (PointsData) is.readObject();

                Forma.initArea(pdata.xCenter, pdata.yCenter, pdata.rXOffset, pdata.rYOffset);

                for (Nokta nokta : pdata.points)
                {
                    chdata.checks.add(Forma.area.contains(nokta.x, nokta.y));
                }

                os.writeObject(chdata);
                os.flush();
                os.reset();
                chdata.checks.clear();
            }

        }
        catch (IOException | ClassNotFoundException e)
        {
            System.out.println("Close connection: " + client.getInetAddress().getHostAddress());
            return;
        }

    }
}
