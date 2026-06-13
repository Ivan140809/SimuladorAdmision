# Simulador de Admisión Universitaria

![Java](https://img.shields.io/badge/Java-25-orange?style=for-the-badge&logo=openjdk&logoColor=white)
![JavaFX](https://img.shields.io/badge/JavaFX-21-007396?style=for-the-badge&logo=java&logoColor=white)
![Maven](https://img.shields.io/badge/Maven-Build-blue?style=for-the-badge&logo=apachemaven&logoColor=white)
![Status](https://img.shields.io/badge/Estado-Finalizado-brightgreen?style=for-the-badge)

Aplicación basado en Java que predice la probabilidad de admisión a la universidad usando regresión lineal. El modelo fue entrenado con datos reales de 400 estudiantes y los coeficientes se cargan desde un archivo JSON generado por un pipeline ETL desarrollado en C++.

## Vista previa

| Pantalla principal | Resultado |
|---|---|
| Formulario con fondo personalizado, campos para GRE, TOEFL, CGPA, etc. | Demostracion  grafica de estudiante Aceptado /No aceptado |

---

## Requisitos

- Java 11 o superior
- Archivo `modelo_parametros_grupo7_corregido.json` en la raíz del proyecto
- Imágenes `space_stars_galaxy_nebula-1918233.jpg`, `feliz.png`, `OIP.jpg` en la raíz para personalizacion 
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
│   │   ├── Main.java              # Ventana principal y lógica de prediccion
│   │   ├── ResultadoVentana.java  # Ventana de resultado (Aceptado / No aceptado)
│   │   ├── Datos.java             # Mapeo del JSON
│   │   └── Coeficientes.java      # Pesos del modelo
│   └── cpp/
│       └── etl_admision.cpp       # Pipeline ETL en C++
├── lib/
│   └── gson-2.11.0.jar            # Librería para leer JSON
├── pom.xml
└── modelo_parametros_grupo7_corregido.json  - requerido para ejecutar
```
## Cómo funciona

1. El usuario ingresa sus datos académicos en el formulario.
2. La app lee los coeficientes del modelo desde el JSON.
3. Aplica la fórmula de regresión lineal
4. Si `z ≥ 0.80` es determinado como Aceptado. Si no es determinado No aceptado.
---

## Parte 1 — Pipeline ETL en C++
El archivo `src/cpp/etl_admision.cpp` implementa el proceso ETL que:
- Lee el dataset CSV de admisiones (400 registros).
- Calcula estadísticas descriptivas y matriz de correlaciones.
- Exporta los resultados a `.txt` y `.bin`.

```bash
g++ -std=c++11 -o etl src/cpp/etl_admision.cpp
./etl
```

---

## Parte 2 — Simulador de Admisión en Java

La aplicación Java usa los coeficientes generados por el ETL (cargados desde el JSON) para predecir la admisión de un estudiante en tiempo real.

**Clases:**
- `Main.java` — construye la ventana con Swing, lee el JSON con Gson y aplica la regresión lineal al hacer clic en el botón.
- `Datos.java` — mapea el JSON a un objeto Java (`intercept`, `coefficients`, `mse`, `r2`).
- `Coeficientes.java` — deserializa los pesos del modelo usando `@SerializedName` (GRE, TOEFL, CGPA, etc.).
- `ResultadoVentana.java` — abre una segunda ventana con imagen y texto según el resultado (`z ≥ 0.80` → Aceptado).

```bash
mvn compile
mvn exec:java -Dexec.mainClass=Main
```
