import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.*;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.util.*;

class ParamArea
{
    static ResourceBundle bundle;

    ParamArea()
    {
        bundle = ResourceBundle.getBundle("ParamArea", Locale.getDefault());

        JFrame mainFrame = new JFrame(bundle.getString("title"));

        mainFrame.setSize(900, 700);
        mainFrame.setLayout(new BorderLayout());
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        CoordinatePlane coordPlane = new CoordinatePlane(125);
        mainFrame.add(coordPlane, BorderLayout.CENTER);

        JPanel usrIntefrace = new JPanel();
        usrIntefrace.setLayout(new BoxLayout(usrIntefrace, BoxLayout.Y_AXIS));
        usrIntefrace.setBorder(BorderFactory.createEmptyBorder(3,3,3,3));
        mainFrame.add(usrIntefrace, BorderLayout.LINE_END);
        GridBagConstraints gridSet = new GridBagConstraints();

        JPanel addNewPointPanel = new JPanel(new GridBagLayout());
        addNewPointPanel.setBorder(BorderFactory.createRaisedSoftBevelBorder());
        addNewPointPanel.setBackground(Color.WHITE);
        gridSet.fill = GridBagConstraints.BOTH;
        gridSet.anchor = GridBagConstraints.CENTER;
        gridSet.weightx = 0.1;
        gridSet.weighty = 0.1;
        gridSet.gridwidth = 2;
        JLabel label = new JLabel(bundle.getString("addNewPoint"), SwingConstants.CENTER);
        label.setBorder(BorderFactory.createMatteBorder(0,0,1,0,Color.DARK_GRAY));
        addNewPointPanel.add(label, gridSet);
        gridSet.gridwidth = 1;
        gridSet.gridy = 1;
        label = new JLabel("x", SwingConstants.CENTER);
        label.setBorder(BorderFactory.createMatteBorder(0,0,0,1,Color.DARK_GRAY));
        addNewPointPanel.add(label, gridSet);
        gridSet.gridx = 1;
        addNewPointPanel.add(new JLabel("y", SwingConstants.CENTER), gridSet);
        JList xCoords = new JList(new DefaultListModel<Integer>());
        JCheckBox yCoords[] = new JCheckBox[coordPlane.getR()*2 + 1];
        Box yBox = Box.createVerticalBox();
        for(int i = -coordPlane.getR(), j = 0; i <= coordPlane.getR(); i++, j++)
        {
            ((DefaultListModel<Integer>)(xCoords.getModel())).addElement(i);
            yCoords[j] = new JCheckBox(String.valueOf(i));
            yCoords[j].setBackground(Color.WHITE);
            yBox.add(yCoords[j]);
        }
        gridSet.gridy++;
        gridSet.gridx = 0;
        gridSet.weightx = 1;
        gridSet.weighty = 1;
        addNewPointPanel.add(new JScrollPane(xCoords), gridSet);
        gridSet.gridx = 1;
        addNewPointPanel.add(new JScrollPane(yBox), gridSet);
        gridSet.weightx = 0.1;
        gridSet.weighty = 0.1;
        gridSet.gridy++;
        gridSet.gridx = 0;
        gridSet.gridwidth = 2;
        JButton addPoint = new JButton(bundle.getString("add"));
        addPoint.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                int[] selectedX = xCoords.getSelectedIndices();

                for(JCheckBox checkBox : yCoords)
                {
                    if(checkBox.isSelected())
                        for (int i = 0; i < selectedX.length; i++)
                        {
                            coordPlane.addPoint( ((DefaultListModel<Integer>)xCoords.getModel()).getElementAt(selectedX[i]),
                                                  Integer.parseInt(checkBox.getText())
                            );
                        }

                        checkBox.setSelected(false);
                }

                xCoords.clearSelection();
            }
        });
        addNewPointPanel.add(addPoint, gridSet);

        JPanel setRPanel = new JPanel(new GridBagLayout());
        setRPanel.setBorder(BorderFactory.createRaisedSoftBevelBorder());
        setRPanel.setBackground(Color.WHITE);
        gridSet.gridwidth = 2;
        gridSet.gridy++;
        gridSet.gridx = 0;
        setRPanel.add(new JLabel(bundle.getString("setR"), SwingConstants.CENTER), gridSet);
        SpinnerModel spinnerModel = new SpinnerNumberModel(125, 1, 1000, 1);
        JSpinner spinnerR = new JSpinner(spinnerModel);
        spinnerR.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                coordPlane.setR((int)((JSpinner)e.getSource()).getValue());
            }
        });
        gridSet.gridy++;
        setRPanel.add(spinnerR, gridSet);

        JPanel infoPointPanel = new JPanel(new GridBagLayout());
        infoPointPanel.setBorder(BorderFactory.createRaisedSoftBevelBorder());
        infoPointPanel.setBackground(Color.WHITE);
        gridSet.gridy++;
        gridSet.gridx = 0;
        infoPointPanel.add(new JLabel(bundle.getString("pointCoords"), SwingConstants.CENTER), gridSet);
        gridSet.gridy++;
        JTextField pointCoord = new JTextField("-");
        pointCoord.setEditable(false);
        pointCoord.setHorizontalAlignment(JTextField.CENTER);
        pointCoord.setBackground(Color.WHITE);
        infoPointPanel.add(pointCoord, gridSet);
        gridSet.gridy++;
        JButton removePoint = new JButton(bundle.getString("delete"));
        removePoint.setEnabled(false);
        removePoint.addActionListener(actionEvent -> {
            coordPlane.removeSelectedPoint();
            pointCoord.setText("-");
            removePoint.setEnabled(false);
        });
        infoPointPanel.add(removePoint, gridSet);
        coordPlane.addPropertyChangeListener(evt -> {
            if(evt.getPropertyName().equals("point"))
            {
                pointCoord.setText( "[" + ((Point)(evt.getNewValue())).getX() + ", " + ((Point)(evt.getNewValue())).getY() + "]");
                removePoint.setEnabled(true);
            }
        });

        usrIntefrace.add(addNewPointPanel);
        usrIntefrace.add(Box.createRigidArea(new Dimension(0, 10)));
        usrIntefrace.add(setRPanel);
        usrIntefrace.add(Box.createRigidArea(new Dimension(0, 10)));
        usrIntefrace.add(infoPointPanel);
        mainFrame.setVisible(true);
    }
}

class CoordinatePlane extends JPanel implements MouseListener
{
    private Area area;
    private HashSet<Ellipse2D> points = new HashSet<Ellipse2D>();
    private Ellipse2D selectedPoint;
    private PointsData pdata;
    private CheckData chdata;
    private Socket servSocket;
    private ObjectOutputStream os;
    private ObjectInputStream is;
    int port;
    String host;

    int r;
    int xCenter;
    int yCenter;
    int rXOffset;
    int rYOffset;
    int pointSize;

    CoordinatePlane(int r)
    {
        this.r = r;
        pointSize = r/13;
        addMouseListener(this);
        setBackground(new Color(245, 245, 220));
        pdata = new PointsData();

        if (Main.port != null)
        {
            try {
                port = Integer.parseInt(Main.port);
            } catch (NumberFormatException e) {
                port = 27000;
            }
        } else
            port = 27000;

        try
        {
            this.host = Main.host != null ? Main.host : InetAddress.getLocalHost().getHostAddress();
            initConnection();

        } catch (IOException e)
        {
            e.printStackTrace();
        }


    }

    public int getR()
    {
        return r;
    }

    public void setR(int r)
    {
        this.r = r;
        repaint();
    }

    private void initArea()
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

    public void addPoint(double x, double y)
    {
        addPointScreen(x*2*rXOffset/r,y*2*rYOffset/r);
    }

    private void addPointScreen(double x, double y)
    {
        Ellipse2D currentPoint = new Ellipse2D.Double((x - pointSize/2)*r/rXOffset, (y - pointSize/2)*r/rYOffset,  pointSize,  pointSize);
        if( !points.add(currentPoint) )
            return;

        pdata.points.clear();
        pdata.addPoint(x + xCenter, yCenter - y);
        if(checkPoints() && chdata.checks.get(0))
        {
            ( new Thread(() -> animatePoint(currentPoint)) ).start();
        }
        else
            repaint();

        selectedPoint = currentPoint;
        pointEvent(new Point( (int)(x*r/(2*rXOffset)), (int)(y*r/(2*rYOffset)) ));
    }

    public void removeSelectedPoint()
    {
        if(selectedPoint != null)
        {
            points.remove(selectedPoint);
            selectedPoint = null;
            repaint();
        }
    }

    @Override
    protected void paintComponent(Graphics g)
    {
        ((Graphics2D)g).setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        super.paintComponent(g);

        xCenter = getWidth() / 2;
        yCenter = getHeight() / 2;
        rXOffset = getWidth() / 6;
        rYOffset = getHeight() / 6;

        g.setColor(Color.BLUE);
        drawArea( (Graphics2D)g );

        g.setColor(Color.BLACK);
        drawCoordinatePlane(g);

        drawPoints((Graphics2D) g);

        if(selectedPoint != null)
        {
            g.setColor(new Color(255, 255, 255, 180));
            g.fillOval((int)(selectedPoint.getX()*rXOffset/r + xCenter), (int)(yCenter - selectedPoint.getY()*rYOffset/r - selectedPoint.getWidth()),
                       (int)(selectedPoint.getWidth()), (int)(selectedPoint.getHeight()) );
        }

    }

    private void drawCoordinatePlane(Graphics g)
    {
        int xOffset = getWidth() / 15;
        int yOffset = getHeight() / 15;
        int arrowOffset = getWidth() / 60;

        g.drawString("y", xCenter + 2*arrowOffset, yOffset);
        g.drawLine(xCenter, getHeight() - yOffset, xCenter, yOffset);
        g.drawPolyline( new int[]{xCenter - arrowOffset, xCenter, xCenter + arrowOffset},
                new int[]{yOffset + arrowOffset, yOffset, yOffset + arrowOffset},
                3);

        g.drawString("x", getWidth() - xOffset, yCenter - 2*arrowOffset);
        g.drawLine(xOffset, yCenter, getWidth() - xOffset, yCenter);
        g.drawPolyline( new int[]{getWidth() - arrowOffset - xOffset, getWidth() - xOffset, getWidth() - arrowOffset - xOffset},
                        new int[]{yCenter + arrowOffset, yCenter, yCenter - arrowOffset},
                3);

        drawXRisk("R/2", arrowOffset,1, g);
        drawXRisk("R", arrowOffset,2, g);
        drawYRisk("R/2", arrowOffset, 1, g);
        drawYRisk("R", arrowOffset, 2, g);
    }

    private void drawXRisk(String label, int riskWidth, int x, Graphics g)
    {
        g.drawString(label, (int) (xCenter + x*rXOffset - g.getFontMetrics().getStringBounds(label, g).getWidth()/2), yCenter - 2*riskWidth);
        g.drawLine(xCenter + x*rXOffset, yCenter - riskWidth, xCenter + x*rXOffset, yCenter + riskWidth);
        label = "-" + label;
        g.drawString(label, (int) (xCenter - x*rXOffset - g.getFontMetrics().getStringBounds(label, g).getWidth()/2), yCenter - 2*riskWidth);
        g.drawLine(xCenter - x*rXOffset, yCenter - riskWidth, xCenter - x*rXOffset, yCenter + riskWidth);
    }

    private void drawYRisk(String label, int riskWidth, int y, Graphics g)
    {
        g.drawString(label, xCenter + 2*riskWidth, yCenter - y*rYOffset + g.getFontMetrics().getAscent()/2);
        g.drawLine(xCenter - riskWidth, yCenter - y*rYOffset, xCenter + riskWidth, yCenter - y*rYOffset);
        label = "-" + label;
        g.drawString(label, xCenter + 2*riskWidth, yCenter + y*rYOffset + g.getFontMetrics().getAscent()/2);
        g.drawLine(xCenter - riskWidth, yCenter + y*rYOffset, xCenter + riskWidth, yCenter + y*rYOffset);
    }

    private void drawArea(Graphics2D g)
    {
       initArea();
       g.fill(area);
    }

    private void drawPoints(Graphics2D g)
    {
        Color incl = Color.GREEN;
        Color excl = Color.red;
        Color unkn = Color.GRAY;

        Ellipse2D drawPoint = new Ellipse2D.Double();

        pdata.points.clear();
        for(Ellipse2D point : points)
        {
            drawPoint.setFrame(point.getX() * rXOffset / r + xCenter, yCenter - point.getY() * rYOffset / r - point.getWidth(), point.getWidth(), point.getHeight());
            pdata.addPoint(drawPoint.getCenterX(), drawPoint.getCenterY());
        }

        if(!checkPoints())
            g.drawString(ParamArea.bundle.getString("cantConnect") + " " + host + ":" + port, 10, 20);

        int i = 0;
        for (Ellipse2D point : points)
        {
            drawPoint.setFrame(point.getX() * rXOffset / r + xCenter, yCenter - point.getY() * rYOffset / r - point.getWidth(), point.getWidth(), point.getHeight());
            g.setColor(chdata != null ? (chdata.checks.get(i) ? incl : excl) : unkn);
            g.fill(drawPoint);
            i++;
        }

    }

    private void animatePoint(Ellipse2D point)
    {
        double step = pointSize / 5;
        int delay = 50;
        double stepX = step * r/(2*rXOffset);
        double stepY = step * r/(2*rYOffset);

        try {

            while (point.getWidth() > pointSize/1.8)
            {
                point.setFrame(point.getX() + stepX, point.getY() + stepY, point.getWidth() - step , point.getHeight() - step);
                repaint();

                Thread.sleep(delay);
            }

            while (point.getWidth() < pointSize)
            {
                point.setFrame(point.getX() - stepX, point.getY() - stepY, point.getWidth() + step, point.getHeight() + step);
                repaint();

                Thread.sleep(delay);
            }

        }
        catch (InterruptedException e)
        {
            return;
        }
    }

    public void mousePressed(MouseEvent me)
    {
        Ellipse2D checkPoint = new Ellipse2D.Double();

        for(Ellipse2D point : points)
        {
            checkPoint.setFrame(point.getX()*rXOffset/r + xCenter, yCenter - point.getY()*rYOffset/r - point.getWidth(), point.getWidth(), point.getHeight());
            if(checkPoint.contains(me.getX(), me.getY()))
            {
                if(selectedPoint == point)
                    return;
                else
                    selectedPoint = point;

                pointEvent(new Point( (int)(point.getCenterX()/2) ,  (int)(point.getCenterY()/2) ) );
                repaint();
                return;
            }
        }

        addPointScreen(me.getX() - xCenter, yCenter - me.getY());
    }

    private void pointEvent(Point point)
    {
        for(PropertyChangeListener listener :  getPropertyChangeListeners ())
        {
            listener.propertyChange(new PropertyChangeEvent(this, "point", null, point));
        }
    }

    public void mouseClicked(MouseEvent e){}
    public void mouseReleased(MouseEvent e){}
    public void mouseEntered(MouseEvent e){}
    public void mouseExited(MouseEvent e){}

    private boolean checkPoints()
    {
        chdata = null;

        try
        {
            pdata.xCenter = xCenter;
            pdata.yCenter = yCenter;
            pdata.rXOffset = rXOffset;
            pdata.rYOffset = rYOffset;

            os.writeObject(pdata);
            os.flush();
            os.reset();

            chdata = (CheckData) is.readObject();
        }
        catch (IOException|ClassNotFoundException|NullPointerException e)
        {
            initConnection();
            return false;
        }

        return true;
    }

    void initConnection()
    {
        try
        {
            servSocket = new Socket(host, port);
            os = new ObjectOutputStream(servSocket.getOutputStream());
            is = new ObjectInputStream(servSocket.getInputStream());
        }
        catch (IOException e)
        {
            return;
        }
    }

}

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


public class Main
{
    static String host;
    static String port;

    public static void main(String[] args)
    {
        host = args.length>0 ? args[0] : null;
        port = args.length>1 ? args[1] : null;

        SwingUtilities.invokeLater(ParamArea::new);
    }
}