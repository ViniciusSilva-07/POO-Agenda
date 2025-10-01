# README — Design e Arquitetura do Projeto Agenda

## Visão Geral

O projeto Agenda foi desenhado para ser modular, extensível e alinhado com os princípios da Programação Orientada a Objetos em C++. A arquitetura separa claramente as responsabilidades entre modelo, controlador, visualização e persistência, facilitando manutenção e evolução.

## Componentes Principais

- **Modelo:**
  - `Event`: Representa um evento, encapsulando título, descrição, horários, tags e recorrência.
  - `Calendar`: Gerencia uma coleção de eventos e pertence a um usuário.
  - `User`: Representa o dono da agenda.
  - `RecurrenceRule`: Interface abstrata para regras de recorrência, permitindo polimorfismo.

- **Interfaces (Abstração):**
  - `IView`: Interface para visualização (CLI/GUI), desacoplando apresentação do modelo.
  - `IController`: Interface para o controlador, responsável por orquestrar operações entre modelo e visualização.
  - `Persistence`: Interface para persistência dos dados, permitindo múltiplas implementações (JSON, SQLite, etc).

- **Stub GUI:**
  - `GuiView`: Implementação futura da interface gráfica, baseada em `IView`.

## Princípios de Design Utilizados

- **Encapsulamento:** Todos os atributos são privados, acessados por getters/setters.
- **Herança e Polimorfismo:** Uso de interfaces abstratas e métodos virtuais para permitir extensibilidade e substituição de comportamentos.
- **Composição:** `Calendar` contém eventos, `Event` pode conter uma regra de recorrência.
- **Modularização:** Separação clara entre camadas e responsabilidades.
- **Uso de STL e Smart Pointers:** Gerenciamento eficiente de recursos e coleções.

## Justificativa das Escolhas

- **Interfaces:** Permitem desacoplar implementação e facilitar testes/mudanças futuras.
- **Smart Pointers:** Evitam vazamentos de memória e facilitam o gerenciamento de objetos dinâmicos.
- **STL:** Proporciona eficiência e simplicidade na manipulação de coleções.
- **Separação de Camadas:** Facilita a evolução do projeto para GUI, persistência avançada e testes automatizados.

## Diagrama de Arquitetura

```
+-------------------+
|      User         |
+-------------------+
        ^
        |
+-------------------+
|    Calendar       |
+-------------------+
        |
        v
+-------------------+
|      Event        |
+-------------------+
        |
        v
+-------------------+
| RecurrenceRule    |
+-------------------+

Interfaces:
IView, IController, Persistence
```

## Evolução Esperada

- Implementação das classes em arquivos `.cpp`.
- Persistência dos dados (JSON/SQLite).
- Interface CLI funcional na etapa 2.
- Interface gráfica (Qt/JUCE) na etapa 3.

## Observações

A arquitetura foi pensada para ser flexível, permitindo fácil expansão e adaptação conforme as próximas etapas do projeto.
