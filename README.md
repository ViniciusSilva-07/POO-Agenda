# Agenda POO — Etapa 2

## Introdução

Este projeto implementa uma **Agenda (planner/organizer)** em C++ como trabalho da disciplina de Programação Orientada a Objetos.  
Na **Etapa 2**, a aplicação está **funcional em terminal (CLI)**, permitindo gerenciamento básico de eventos com persistência em JSON, além de aplicar os principais conceitos de POO e boas práticas modernas da linguagem.

---

## Funcionalidades Implementadas (Etapa 2)

- Criar eventos (título, descrição, início, fim e tags).  
- Listar eventos ordenados por data/hora.  
- Salvar eventos em arquivo JSON.  
- Carregar eventos a partir de arquivo JSON.  
- Suporte a recorrência diária simples (`RecurrenceRuleDaily`).  
- Tratamento de exceções (datas inválidas, erros de I/O).  

---

## Estrutura Entregue

- **Modelo**: `User`, `Calendar`, `Event`, `RecurrenceRule`, `RecurrenceRuleDaily`.  
- **Controle**: `IController`, `Controller`.  
- **Visualização**: `IView`, `ConsoleView` (CLI).  
- **Persistência**: `IPersistence`, `JSONPersistence`.  
- **Utilitários**: `TimeUtils`, `UtilTemplates`.  

Outros arquivos:  
- `CMakeLists.txt` → configuração de build.  
- `main.cpp` → ponto de entrada da aplicação.  
- `README.md` (este arquivo).  
- `README_DESIGN.md` (detalhes da arquitetura e justificativas de design).  

---

## Classes Principais e Relações

- **User** → dono da agenda.  
- **Calendar** → contém os eventos de um usuário.  
- **Event** → representa um evento (título, descrição, horários, tags, recorrência).  
- **RecurrenceRule** → interface abstrata para regras de recorrência.  
- **RecurrenceRuleDaily** → regra de recorrência concreta (diária).  
- **IView / ConsoleView** → visualização em terminal.  
- **IController / Controller** → coordena modelo, visão e persistência.  
- **IPersistence / JSONPersistence** → salva e carrega os eventos em JSON.  

---

## Diagrama Simplificado

+-------------------+ +-------------------+ +-------------------+
| User | | IView |<-------| ConsoleView |
+-------------------+ +-------------------+ +-------------------+
| ^
v |
+-------------------+ +-------------------+ +-------------------+
| Calendar |<------>| IController |<-------| Controller |
+-------------------+ +-------------------+ +-------------------+
|
v
+-------------------+
| Event |
+-------------------+
|
v
+-------------------+ +-------------------+
| RecurrenceRule |<-------| RecurrenceDaily |
+-------------------+ +-------------------+

Persistência:
+-------------------+ +-------------------+
| IPersistence |<-------| JSONPersistence |
+-------------------+ +-------------------+

## Como Executar

 Usando CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
./agenda   # Linux / Mac
agenda.exe # Windows


Para detalhes completos da arquitetura e justificativas de design, consulte o arquivo README_DESIGN.md.