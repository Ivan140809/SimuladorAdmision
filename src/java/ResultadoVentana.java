import javax.swing.*;
import java.awt.*;

public class ResultadoVentana {

    public static void mostrarResultado(double z) {
        JFrame frame = new JFrame("Resultado de Evaluación");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 350);
        frame.setLocationRelativeTo(null);

        JLabel label = new JLabel();
        label.setHorizontalAlignment(JLabel.CENTER);
        label.setVerticalAlignment(JLabel.CENTER);
        label.setFont(new Font("Arial", Font.BOLD, 20));

        String texto;
        String rutaImg;
        if (z >= 0.80) {
            texto = "¡Aceptado!";
            rutaImg = "feliz.png";
        } else {
            texto = "No fue aceptado";
            rutaImg = "OIP.jpg";
        }

        ImageIcon iconoOriginal = new ImageIcon(rutaImg);
        Image imgEscalada = iconoOriginal.getImage()
                .getScaledInstance(150, 150, Image.SCALE_SMOOTH);
        label.setIcon(new ImageIcon(imgEscalada));

        label.setText(texto);
        label.setHorizontalTextPosition(SwingConstants.CENTER);
        label.setVerticalTextPosition(SwingConstants.BOTTOM);
        label.setIconTextGap(15);

        frame.add(label);
        frame.setVisible(true);
    }
}
