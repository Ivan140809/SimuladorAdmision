# Simulador de Admisión Universitaria

Aplicación basado en java en Java que predice la probabilidad de admisión a un posgrado usando regresión lineal. El modelo fue entrenado con datos reales de 400 estudiantes y los coeficientes se cargan desde un archivo JSON generado por el pipeline ETL en C++.

---

## Vista previa

| Pantalla principal | Resultado |
|---|---|
| Formulario con fondo personalizado, campos para GRE, TOEFL, CGPA, etc. | Demostracion  grafica de estudiante Aceptado /No aceptado |

---

## Requisitos

- Java 11 o superior
- Archivo `modelo_parametros_grupo7_corregido.json` en la raíz del proyecto
- Imágenes `space_stars_galaxy_nebula-1918233.jpg`, `feliz.png`, `OIP.jpg` en la raíz

---

## Cómo ejecutar

### Opción 1 — Maven
```bash
mvn compile
mvn exec:java -Dexec.mainClass=Main
```

### Opción 2 — Compilación directa
```bash
javac -cp lib/gson-2.11.0.jar src/java/*.java -d bin/
java -cp bin:lib/gson-2.11.0.jar Main
```

---

## Estructura del proyecto

```
├── src/
│   ├── java/
│   │   ├── Main.java              # Ventana principal y lógica de predicción
│   │   ├── ResultadoVentana.java  # Ventana de resultado (Aceptado / No aceptado)
│   │   ├── Datos.java             # Mapeo del JSON
│   │   └── Coeficientes.java      # Pesos del modelo (deserialización Gson)
│   └── cpp/
│       └── etl_admision.cpp       # Pipeline ETL en C++
├── lib/
│   └── gson-2.11.0.jar            # Librería para leer JSON
├── pom.xml
└── modelo_parametros_grupo7_corregido.json   ← requerido para ejecutar
```

---

## Cómo funciona

1. El usuario ingresa sus datos académicos en el formulario.
2. La app lee los coeficientes del modelo desde el JSON.
3. Aplica la fórmula de regresión lineal: `z = Σ(valor × coeficiente) + intercepto`.
4. Si `z ≥ 0.80` → **Aceptado**. Si no → **No aceptado**.

---

## Parte 1 — Pipeline ETL en C++

El archivo `src/etl_admision.cpp` implementa el proceso ETL que:
- Lee el dataset CSV de admisiones (400 registros).
- Calcula estadísticas descriptivas y matriz de correlaciones.
- Exporta los resultados a `.txt` y `.bin`.

```bash
g++ -std=c++11 -o etl etl_admision.cpp
./etl
```
