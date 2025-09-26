# Agenda POO — Etapa 1

## Introdução

Este projeto implementa uma **Agenda (planner/organizer)** em C++ como trabalho da disciplina de Programação Orientada a Objetos. O objetivo é permitir o gerenciamento de eventos, com funcionalidades de recorrência, busca, filtragem e persistência, explorando os principais conceitos de POO e boas práticas modernas da linguagem.

## Estrutura Entregue
- Headers das principais classes e interfaces (`Event.h`, `Calendar.h`, `RecurrenceRule.h`, `User.h`, `IView.h`, `IController.h`, `Persistence.h`, `GuiView.h`)
- `CMakeLists.txt` para build automatizado
- `README.md` (este arquivo)
- `README_DESIGN.md` (detalhes da arquitetura e decisões de design)
- Diagrama UML (no README e no README_DESIGN)

## Classes Principais e Relações
- **User**: representa o dono da agenda.
- **Calendar**: pertence a um usuário e gerencia uma coleção de eventos.
- **Event**: representa um evento, com título, descrição, horários, tags e recorrência.
- **RecurrenceRule**: interface abstrata para regras de recorrência, permitindo polimorfismo e extensibilidade.
- **IView**: interface para visualização (CLI/GUI), desacoplando apresentação do modelo.
- **IController**: interface para o controlador, responsável por orquestrar operações entre modelo e visualização.
- **Persistence**: interface para persistência dos dados, permitindo múltiplas implementações (JSON, SQLite, etc).

## Diagrama UML (Simplificado)

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

## Objetivo da Etapa 1
- Garantir que a arquitetura atenda os requisitos de **POO**:
  - Encapsulamento (atributos privados, getters/setters)
  - Herança e polimorfismo (`RecurrenceRule`, `IView`, `IController`, `Persistence`)
  - Composição (`Calendar` contém `Event`)
  - Sobrecarga de operadores (`Event` compara datas)
  - Uso de STL (`std::vector`) e smart pointers (`std::shared_ptr`)
  - Modularização e separação clara de responsabilidades

## Evolução do Projeto
O projeto está pronto para evoluir nas próximas etapas:
- **Etapa 2:** Implementação funcional em terminal (CLI), persistência básica, tratamento de exceções
- **Etapa 3:** Interface gráfica (GUI), persistência avançada, documentação final

Para detalhes completos da arquitetura e justificativas de design, consulte o arquivo `README_DESIGN.md`.
