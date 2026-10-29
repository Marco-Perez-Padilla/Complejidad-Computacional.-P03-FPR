# Función Potencia como Función Primitiva Recursiva

**Autor:** Marco Pérez Padilla  
**Correo:** alu0101469348@ull.edu.es  
**Asignatura:** Complejidad Computacional — Curso 2026/27  
**Práctica 3:** Implementación de funciones primitivas recursivas de números naturales

Programa orientado a objetos, escrito en C++, que calcula `potencia(x, y) = xʸ`
como una **función primitiva recursiva**: construida únicamente con las
funciones básicas (cero, sucesor y proyecciones) y los operadores de
composición y recursión primitiva. Muestra el resultado y el número de
llamadas a funciones realizadas.

## Índice

- [Definición de las funciones](#definición-de-las-funciones)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Diseño y decisiones técnicas](#diseño-y-decisiones-técnicas)
- [Cuenta de llamadas](#cuenta-de-llamadas)
- [Resultados](#resultados)
- [Recursión simulada frente a recursión real](#recursión-simulada-frente-a-recursión-real)
- [Manejo de errores](#manejo-de-errores)
- [Compilación y ejecución](#compilación-y-ejecución)
- [Tests](#tests)
- [Contenedor Docker](#contenedor-docker)
- [Repositorio](#repositorio)

## Definición de las funciones

Funciones básicas: `Zᵏ(x₁..xₖ) = 0`, `S(x) = x + 1` y `Pᵏᵢ(x₁..xₖ) = xᵢ`.

Operadores:

- **Composición** `Cn[h; g₁..gₘ](x̄) = h(g₁(x̄), ..., gₘ(x̄))`.
- **Recursión primitiva** `Pr[g, h]`: `f(x̄, 0) = g(x̄)` y
  `f(x̄, y+1) = h(x̄, y, f(x̄, y))`.

A partir de ellos (los tres están en `FunctionLibrary`):

| Función | Definición recursiva | Notación |
|---|---|---|
| `suma(x, y)` | `suma(x, 0) = P¹₁(x)`<br>`suma(x, y+1) = S(P³₃(x, y, suma(x, y)))` | `Pr[P¹₁, Cn[S; P³₃]]` |
| `producto(x, y)` | `producto(x, 0) = Z¹(x)`<br>`producto(x, y+1) = suma(producto(x, y), x)` | `Pr[Z¹, Cn[suma; P³₃, P³₁]]` |
| `potencia(x, y)` | `potencia(x, 0) = S(Z¹(x))`<br>`potencia(x, y+1) = producto(potencia(x, y), x)` | `Pr[Cn[S; Z¹], Cn[producto; P³₃, P³₁]]` |

Por definición `potencia(0, 0) = 1`.

## Estructura del proyecto

```
.
├── Makefile
├── README.md
├── include/            # cabeceras (.h), organizadas por responsabilidad
│   ├── exceptions/      # jerarquía de excepciones (exceptions.h)
│   ├── model/           # Natural (tipo de los números naturales)
│   ├── functions/       # Function y sus subclases, CallCounter, FunctionLibrary
│   ├── io/              # Application, Options
│   └── help/            # Help, Usage, PrintWarning/PrintError, ValidateArguments
├── src/                # implementación (.cc), misma organización que include/
├── tests/              # tests de GoogleTest
└── libs/googletest/     # código fuente de GoogleTest
```

## Diseño y decisiones técnicas

- **`Function`** es la clase abstracta de toda función: `Arity()`,
  `Describe()` y `Evaluate(args, counter)`. `Evaluate` no es virtual
  (patrón *Template Method*): comprueba que el número de argumentos es la
  aridad, cuenta la llamada y delega en `DoEvaluate`, que es lo único que
  implementan las subclases. Así ninguna subclase puede olvidar contar.
- **Funciones básicas**: `ZeroFunction`, `SuccessorFunction`,
  `ProjectionFunction`.
- **Operadores**: `CompositionFunction` y `PrimitiveRecursionFunction`
  reciben las funciones que combinan (`FunctionPtr` = `shared_ptr`, porque la
  suma está dentro del producto, que está dentro de la potencia) y validan las
  aridades en el constructor.
- **`FunctionLibrary`** construye suma, producto y potencia como un árbol de
  objetos de función, no como subclases que envuelven a otra: de esa forma
  cada nodo de la definición se evalúa y se cuenta exactamente una vez.
- **`CallCounter`** se pasa por referencia a lo largo de la evaluación en
  lugar de ser una variable global; las funciones no tienen estado y cada
  cómputo tiene su propia cuenta.
- **Recursión primitiva simulada con un bucle**: `PrimitiveRecursionFunction`
  no se llama a sí misma; evalúa con un bucle de 0 a `y`, que es el
  desarrollo de la definición recursiva (`f(x̄, 1)`, `f(x̄, 2)`, ...). Es
  una decisión consciente y se explica en
  [Recursión simulada frente a recursión real](#recursión-simulada-frente-a-recursión-real).
  Las llamadas se cuentan igual que si se hubiera aplicado la definición de
  forma recursiva (ver [Cuenta de llamadas](#cuenta-de-llamadas)).
- **`Natural`** es `uint64_t`. El sucesor es la única función que hace
  crecer los números, así que es el único sitio donde se detecta el
  desbordamiento.
- **`Application`** recibe el flujo de salida por inyección (por defecto
  `std::cout`), como en las prácticas anteriores, para poder probarla con un
  `ostringstream`.

## Cuenta de llamadas

Cada evaluación de cualquier función de la definición (básica, composición o
recursión) es **una llamada**. Para `f = Pr[g, h]`, calcular `f(x̄, y)` son
`y + 1` llamadas a `f`, una a `g` y `y` a `h` (más todo lo que `h` llame).

De ahí salen fórmulas cerradas, comprobadas en los tests:

| Función | Llamadas |
|---|---|
| `suma(x, y)` | `4y + 2` |
| `producto(x, y)` | `2 + y·(4x + 6)` |
| `potencia(x, 0)` | `4` |
| `potencia(x, y+1)` | `potencia(x, y)` + `6 + 6x + 4·xʸ⁺¹` |

Ejemplo: `potencia(2, 10)` = 4 + Σₖ₌₁¹⁰ (6 + 12 + 4·2ᵏ) = **8368** llamadas.

El número de llamadas crece con el propio resultado (la potencia se
calcula a base de sumas de uno en uno), por lo que `2^20` ya son 8,4
millones de llamadas.

## Resultados

Salidas obtenidas (la columna de llamadas coincide con las fórmulas de
arriba):

| Comando | Resultado | Llamadas |
|---|---|---|
| `-x 10 -y 0` | 1 | 4 |
| `-x 5 -y 2` | 25 | 196 |
| `-x 3 -y 3` | 27 | 232 |
| `-x 2 -y 5` | 32 | 342 |
| `-x 2 -y 10` | 1 024 | 8 368 |
| `-x 2 -y 15` | 32 768 | 262 410 |
| `-x 2 -y 20` | 1 048 576 | 8 388 964 |
| `-x 2 -y 25 -limit 1000000000` | 33 554 432 | 268 435 902 |
| `-x 2 -y 30 -limit 10000000000` | 1 073 741 824 | 8 589 935 128 |
| `-x 2 -y 40 -limit 10000000000000` | 1 099 511 627 776 (*) | 8 796 093 022 924 (*) |

(*) Valor teórico, no ejecutado hasta el final: el resultado es 2⁴⁰ y las
llamadas salen de la fórmula de la sección anterior (que reproduce exactamente
las ejecuciones de `2^25` y `2^30`). Como `2^40` hace unas 1 000 veces más
llamadas que `2^30`, tardaría aproximadamente 5 días en este equipo.

Con los límites por defecto, `-x 2 -y 25` ya supera las 50 000 000
llamadas y el programa lo indica con un error. `2^30` tarda unos minutos y
`2^40`, días.

## Recursión simulada frente a recursión real

En esta implementación **ninguna función se llama a sí misma**. Las llamadas
anidadas de C++ son siempre de una función a otra distinta y hacia "dentro" de
la definición (`Pr_potencia → Cn_producto → Pr_producto → Cn_suma → Pr_suma →
Cn_S → S`), un anidamiento fijo que no depende de `x` ni de `y`. La única
recursión que se desenrolla es la de `Pr[g, h]`: en vez de que `f(x̄, y)` llame
a `f(x̄, y-1)`, el bucle de `PrimitiveRecursionFunction::DoEvaluate` va
calculando `f(x̄, 0)`, `f(x̄, 1)`, ... guardando el valor anterior. Cada una de
esas llamadas que no se hacen se suma igualmente al contador, así que el
**resultado y el número de llamadas son exactamente los de la recursión
real**.

Se eligió así porque con recursión real el programa deja de ser utilizable
muy pronto. La suma recursiona sobre su segundo argumento y, dentro del
producto, ese argumento es el propio resultado parcial (`suma(acc, xʸ)`), de
modo que la profundidad de la pila es del orden del resultado. Con la pila por
defecto (8 MB) la versión recursiva funciona hasta `2^15` y da *segmentation
fault* desde `2^16`. Para `2^30` harían falta del orden de 10⁹ niveles de
llamadas, cientos de GB de pila. La versión con bucle usa pila constante y solo
está limitada por el tiempo.

### Cómo sería la versión con recursión real

Solo hay que cambiar `PrimitiveRecursionFunction::DoEvaluate`
(`src/functions/primitive_recursion_function.cc`). El resto del proyecto, el
contador incluido, no cambia: la llamada recursiva pasa por `Evaluate`, que ya
se cuenta sola.

```cpp
Natural PrimitiveRecursionFunction::DoEvaluate(const Arguments& args,
                                               CallCounter& counter) const {
  const std::size_t n = args.size() - 1;
  const Natural y = args.back();
  Arguments fixed(args.begin(), args.begin() + n);

  // f(x, 0) = g(x)
  if (y == 0) return base_->Evaluate(fixed, counter);

  // f(x, y) = h(x, y - 1, f(x, y - 1))
  Arguments previous = fixed;
  previous.push_back(y - 1);
  Natural value = Evaluate(previous, counter);  // f(x, y - 1): recursión real

  Arguments step_arguments = fixed;
  step_arguments.push_back(y - 1);
  step_arguments.push_back(value);
  return step_->Evaluate(step_arguments, counter);
}
```

Además, en `include/functions/primitive_recursion_function.h` hay que cambiar
el comentario de la clase (ya no es un bucle) y, en
`tests/functions/operators_test.cc`, sustituir el test que comprueba que un
valor grande no desborda la pila, que dejaría de ser cierto:

```cpp
// Antes (versión con bucle):
TEST(PrimitiveRecursionFunctionTest, LargeVariableDoesNotOverflowTheStack) {
  FunctionPtr f = FunctionLibrary::PrimitiveRecursion(
      FunctionLibrary::Zero(1), FunctionLibrary::Projection(3, 3));
  CallCounter counter(10'000'000);
  EXPECT_EQ(f->Evaluate({1, 1'000'000}, counter), 0u);
}

// Después (versión recursiva):
TEST(PrimitiveRecursionFunctionTest, ModerateVariableWorks) {
  FunctionPtr f = FunctionLibrary::PrimitiveRecursion(
      FunctionLibrary::Zero(1), FunctionLibrary::Projection(3, 3));
  CallCounter counter(10'000'000);
  EXPECT_EQ(f->Evaluate({1, 1'000}, counter), 0u);
}
```

Con ese cambio siguen pasando todos los demás tests y se obtienen las mismas
salidas y los mismos números de llamadas hasta `2^15`; a partir de `2^16`
el programa termina con *segmentation fault* en lugar de un resultado.

## Manejo de errores

Todas las excepciones derivan de `FprException` y `main` las captura,
muestra `Error: ...` y termina con código 1.

| Situación | Excepción / comportamiento |
|---|---|
| Falta `-x` o `-y`, opción desconocida, opción repetida, opción sin valor | mensaje + `Usage`, código 1 |
| Valor que no es un natural (negativo, decimal, texto, mayor que 2⁶⁴−1) | mensaje + `Usage`, código 1 |
| `-limit 0` | mensaje + `Usage`, código 1 |
| Resultado mayor que 2⁶⁴−1 | `NaturalOverflowException` |
| Más llamadas que el límite | `CallLimitExceededException` |
| Función mal construida (aridades incompatibles, proyección fuera de rango, función nula) | `InvalidFunctionException` |
| Evaluar con un número de argumentos distinto de la aridad | `InvalidArityException` |

## Compilación y ejecución

```bash
make            # compila bin/fpr
./bin/fpr -x 2 -y 10
```

Opciones:

| Opción | Descripción |
|---|---|
| `-x <n>` | Base de la potencia (natural, obligatoria) |
| `-y <n>` | Exponente de la potencia (natural, obligatoria) |
| `-limit <n>` | Máximo de llamadas permitidas (opcional, 50 000 000 por defecto) |
| `--help`, `-h` | Muestra la ayuda |

Ejemplos de salida:

```
$ ./bin/fpr -x 2 -y 10
power(2, 10) = 1024
Function calls: 8368

$ ./bin/fpr -x 0 -y 0
power(0, 0) = 1
Function calls: 4

$ ./bin/fpr -x 10 -y 10
Error: the computation exceeded the maximum number of function calls (50000000); use -limit to raise it
```

`-limit` es una opción extra (el enunciado solo pide `-x` y `-y`): como el
coste crece tan deprisa, evita que una entrada grande deje el programa
trabajando prácticamente para siempre.

## Tests

Usan GoogleTest (`libs/googletest`).

```bash
make test
```

Cubren las funciones básicas, los dos operadores (incluidos todos los errores
de construcción), suma, producto y potencia (valores y número exacto de
llamadas contra las fórmulas), el contador, las excepciones, `Application`,
el análisis de números naturales y la validación de argumentos.

## Contenedor Docker

```bash
docker compose up -d --build
docker compose exec cpp-dev bash
make && ./bin/fpr -x 2 -y 10
```

## Repositorio

<https://github.com/Marco-Perez-Padilla/Complejidad-Computacional.-P03-FPR>