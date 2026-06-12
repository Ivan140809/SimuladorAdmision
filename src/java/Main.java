import com.google.gson.Gson;

import javax.swing.*;
import java.awt.*;
import java.io.FileReader;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(Main::crearVentana);
    }

    public static void crearVentana() {
        JFrame frame = new JFrame("Simulador de Admisión");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(500, 700);

        ImageIcon imagenFondo = new ImageIcon("space_stars_galaxy_nebula-1918233.jpg");
        JLabel fondo = new JLabel(imagenFondo);
        fondo.setLayout(new GridLayout(9, 2, 5, 5));
        frame.setContentPane(fondo);

        JTextField greField = new JTextField();
        JTextField toeflField = new JTextField();
        JTextField ratingField = new JTextField();
        JTextField sopField = new JTextField();
        JTextField lorField = new JTextField();
        JTextField cgpaField = new JTextField();
        JTextField researchField = new JTextField();

        JLabel labelGRE = new JLabel("GRE Score:");
        labelGRE.setForeground(Color.WHITE);
        fondo.add(labelGRE); fondo.add(greField);

        JLabel labelTOEFL = new JLabel("TOEFL Score:");
        labelTOEFL.setForeground(Color.WHITE);
        fondo.add(labelTOEFL); fondo.add(toeflField);

        JLabel labelRating = new JLabel("University Rating:");
        labelRating.setForeground(Color.WHITE);
        fondo.add(labelRating); fondo.add(ratingField);

        JLabel labelSOP = new JLabel("SOP:");
        labelSOP.setForeground(Color.WHITE);
        fondo.add(labelSOP); fondo.add(sopField);

        JLabel labelLOR = new JLabel("LOR:");
        labelLOR.setForeground(Color.WHITE);
        fondo.add(labelLOR); fondo.add(lorField);

        JLabel labelCGPA = new JLabel("CGPA:");
        labelCGPA.setForeground(Color.WHITE);
        fondo.add(labelCGPA); fondo.add(cgpaField);

        JLabel labelResearch = new JLabel("Research (0 o 1):");
        labelResearch.setForeground(Color.WHITE);
        fondo.add(labelResearch); fondo.add(researchField);

        JButton btnCalcular = new JButton("Calcular Admisión");
        JLabel resultadoLabel = new JLabel("Probabilidad: ");

        btnCalcular.setForeground(Color.WHITE);
        resultadoLabel.setForeground(Color.WHITE);
        btnCalcular.setBackground(Color.gray);
        btnCalcular.setOpaque(true);
        btnCalcular.setBorderPainted(false);
        frame.add(btnCalcular);
        frame.add(resultadoLabel);

        btnCalcular.addActionListener(e -> {
            try {
                Gson gson = new Gson();
                FileReader reader = new FileReader("modelo_parametros_grupo7_corregido.json");
                Datos pesos = gson.fromJson(reader, Datos.class);
                reader.close();

                System.out.println("Coeficientes cargados:");
                System.out.println("GRE: " + pesos.coefficients.gre);
                System.out.println("TOEFL: " + pesos.coefficients.toefl);
                System.out.println("CGPA: " + pesos.coefficients.cgpa);
                System.out.println("Intercept: " + pesos.intercept);

                double gre = Double.parseDouble(greField.getText());
                double toefl = Double.parseDouble(toeflField.getText());
                double rating = Double.parseDouble(ratingField.getText());
                double sop = Double.parseDouble(sopField.getText());
                double lor = Double.parseDouble(lorField.getText());
                double cgpa = Double.parseDouble(cgpaField.getText());
                double research = Double.parseDouble(researchField.getText());

                double z = gre * pesos.coefficients.gre +
                        toefl * pesos.coefficients.toefl +
                        rating * pesos.coefficients.rating +
                        sop * pesos.coefficients.sop +
                        lor * pesos.coefficients.lor +
                        cgpa * pesos.coefficients.cgpa +
                        research * pesos.coefficients.research +
                        pesos.intercept;

                double probabilidad = z;

                resultadoLabel.setText(String.format("Probabilidad: %.2f%%", probabilidad * 100));
                ResultadoVentana.mostrarResultado(z);

            } catch (Exception ex) {
                JOptionPane.showMessageDialog(frame, "Error en los datos o al leer el JSON");
                ex.printStackTrace();
            }
        });

        frame.setVisible(true);
    }
}
