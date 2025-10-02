## Visão Geral

O projeto **Agenda** foi desenhado para ser modular, extensível e alinhado com os princípios da Programação Orientada a Objetos em C++.  
A arquitetura separa claramente as responsabilidades entre **modelo**, **controlador**, **visualização** e **persistência**, facilitando manutenção e evolução.  
Na versão 2, a aplicação é executada via **terminal (CLI)**, atendendo aos requisitos.  

---

## Componentes Principais

- **Modelo**
  - `Event`: Representa um evento, encapsulando título, descrição, horários, tags e recorrência.
  - `Calendar`: Gerencia uma coleção de eventos e pertence a um usuário.
  - `User`: Representa o dono da agenda.
  - `RecurrenceRule`: Interface abstrata para regras de recorrência, permitindo polimorfismo.
  - `RecurrenceRuleDaily`: Implementação concreta da regra de recorrência diária.

- **Controle**
  - `IController`: Interface para o controlador, responsável por orquestrar operações entre modelo e visualização.
  - `Controller`: Implementação concreta, que manipula eventos e delega operações de persistência e visualização.

- **Visualização**
  - `IView`: Interface para a visualização (abstrata).
  - `ConsoleView`: Implementação em terminal (versão 2).
  - `GuiView`: Implementação futura da interface gráfica (versão 3).

- **Persistência**
  - `IPersistence`: Interface para persistência de dados, abstraindo o formato de armazenamento.
  - `JSONPersistence`: Implementação concreta que salva/carrega eventos em formato JSON.

- **Utilitários**
  - `TimeUtils`: Converte datas entre `std::chrono::system_clock::time_point` e strings no formato `YYYY-MM-DD HH:MM`.
  - `UtilTemplates`: Exemplo de template genérico (`join`) para concatenar listas de strings.

---

## Princípios de Design Utilizados

- **Encapsulamento**: todos os atributos são privados e acessados por getters e setters.  
- **Herança e Polimorfismo**: `RecurrenceRule` (abstrata) e `RecurrenceRuleDaily` (concreta).  
- **Composição**: `Calendar` mantém uma coleção de `Event`.  
- **Interfaces (contratos)**: `IView`, `IController`, `IPersistence` permitem baixo acoplamento e substituição de implementações.  
- **Sobrecarga de Operadores**: `Event::operator<` para ordenação cronológica e `Event::operator==` para comparação.  
- **Exceções**: tratam erros de entrada (datas inválidas) e problemas de persistência.  
- **Smart Pointers**: uso de `std::shared_ptr<Event>` para gerenciamento seguro de memória.  
- **Templates**: função `join` demonstra genericidade.  
- **Separação de Camadas (MVC simplificado)**:  
  - Modelo: `User`, `Calendar`, `Event`, `RecurrenceRule`.  
  - Controle: `Controller`.  
  - Visão: `ConsoleView` (versão 2), `GuiView` (versão 3).  

---

## Justificativa das Escolhas

- Uso de **interfaces** desacopla implementação do contrato, permitindo evolução futura (ex.: trocar `JSONPersistence` por `SQLitePersistence`).  
- O formato **JSON** foi escolhido pela simplicidade, legibilidade e facilidade de parsing.  
- A escolha de **CLI na versão 2** segue o requisito da disciplina, mas já há previsão para **GUI na versão 3**.  
- **Smart pointers** evitam vazamentos de memória e tornam o código mais seguro.  
- A separação em múltiplos arquivos `.h` e `.cpp` garante modularidade e facilita manutenção.  
- O design MVC simplificado organiza responsabilidades de forma clara e didática.  

---

## Diagrama de Arquitetura

+-------------------+ +-------------------+ +-------------------+
| User | | IView |<-------| ConsoleView |
+-------------------+ +-------------------+ +-------------------+
| ^ ^
v | |
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

## Fluxo de Execução (Versão 2 – CLI)

1. O programa inicia pedindo nome e email do usuário.  
2. Exibe um menu com as opções:  

1 - criar evento
2 - listar eventos
3 - salvar
4 - carregar
0 - sair

3. Usuário pode criar eventos, listar, salvar em JSON e carregar do arquivo.  
4. Eventos são exibidos ordenados por data/hora.  

---

## Checklist da Versão 2

- Estrutura modular (`.h` + `.cpp`, `main.cpp` na raiz).  
- Agenda funcional em terminal (CLI).  
- Criação e listagem de eventos.  
- Salvamento e carregamento em arquivo JSON.  
- Classes com herança, polimorfismo, composição.  
- Exceções e tratamento de erros.  
- Uso de smart pointers (`shared_ptr`).  
- Sobrecarga de operadores.  
- Função template (`join`).  