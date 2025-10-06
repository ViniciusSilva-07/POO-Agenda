## Visão Geral

O projeto **Agenda** foi desenhado para ser modular, extensível e alinhado com os princípios da Programação Orientada a Objetos em C++.  
A arquitetura separa claramente as responsabilidades entre **modelo**, **controlador**, **visualização** e **persistência**, facilitando manutenção e evolução.  
Atualmente a interface principal é gráfica (Win32 GUI) e a aplicação abre diretamente a GUI ao iniciar.

---

## Componentes Principais

- **Modelo**
  - `Event`: Representa um evento, encapsulando título, descrição, horários, tags e recorrência.
  - `Calendar`: Gerencia uma coleção de eventos e pertence a um usuário.
  - `User`: Representa o dono da agenda.
  - `RecurrenceRule`: Interface abstrata para regras de recorrência, permitindo polimorfismo.
  - `RecurrenceRuleDaily`: Implementação concreta da regra de recorrência diária.

- **Controle**
  - `Controller`: Responsável por orquestrar operações entre modelo e visualização, gerenciar persistência e fornecer listas formatadas para a GUI.

- **Visualização**
  - `IView`: Interface para a visualização (abstrata).
  - `GUIView`: Implementação principal baseada na API Win32 (abertura direta da janela, formulários e listagem).
  - `ConsoleView`: Mantido no repositório como implementação alternativa, mas não é invocado no fluxo padrão.

- **Persistência**
  - `IPersistence`: Interface para persistência de dados, abstraindo o formato de armazenamento.
  - `JSONPersistence`: Implementação concreta que salva/carrega eventos em formato JSON. O carregador aceita datas no formato `DD-MM-YYYY HH:MM` além do formato interno.

- **Utilitários**
  - `TimeUtils`: Converte datas entre `std::chrono::system_clock::time_point` e strings; internamente usa `YYYY-MM-DD HH:MM`, mas a interface exibe `DD-MM-YYYY`.
  - `UtilTemplates`: Exemplo de template genérico (`join`) para concatenar listas de strings.

---

## Princípios de Design Utilizados

- **Encapsulamento**: todos os atributos são privados e acessados por getters e setters.  
- **Herança e Polimorfismo**: `RecurrenceRule` (abstrata) e `RecurrenceRuleDaily` (concreta).  
- **Composição**: `Calendar` mantém uma coleção de `Event`.  
- **Interfaces (contratos)**: `IView`, `IPersistence` permitem baixo acoplamento e substituição de implementações.  
- **Sobrecarga de Operadores**: `Event::operator<` para ordenação cronológica e `Event::operator==` para comparação.  
- **Exceções**: tratam erros de entrada (datas inválidas) e problemas de persistência.  
- **Smart Pointers**: uso de `std::shared_ptr<Event>` para gerenciamento seguro de memória.  
- **Templates**: função `join` demonstra genericidade.  
- **Separação de Camadas (MVC simplificado)**:  
  - Modelo: `User`, `Calendar`, `Event`, `RecurrenceRule`.  
  - Controle: `Controller`.  
  - Visão: `GUIView` (principal).  

---

## Justificativa das Escolhas

- Uso de **interfaces** desacopla implementação do contrato, permitindo evolução futura (ex.: trocar `JSONPersistence` por `SQLitePersistence`).  
- O formato **JSON** foi escolhido pela simplicidade, legibilidade e facilidade de parsing.  
- A interface gráfica foi implementada com Win32 API para uma aplicação desktop leve e sem dependências externas.

---

## Diagrama de Arquitetura (simplificado)

+-------------------+     +-------------------+     +-------------------+
| User              |     | IView             |<----| GUIView           |
+-------------------+     +-------------------+     +-------------------+
      ^
      |
+-------------------+     +-------------------+     +-------------------+
| Calendar          |<--->| Controller        |<----| Persistence (JSON)|
+-------------------+     +-------------------+     +-------------------+
      |
      v
  +-------------------+
  | Event             |
  +-------------------+

## Fluxo de Execução (GUI)

1. O programa inicia e abre diretamente a janela gráfica principal.
2. A GUI permite criar eventos via formulário com campos separados para data (DD-MM-YYYY) e hora (HH:MM).
3. Botões principais:
   - "Criar": cria o evento com os dados do formulário e limpa os campos.
   - "Eventos": atualiza a lista principal (mesma saída que a listagem padrão).
   - "Eventos (Mes)": agrupa eventos por mês e exibe mês por extenso (sem acentos) com uma linha em branco entre meses.
   - "Salvar": abre o diálogo de salvar e grava os eventos em JSON.
   - "Carregar": abre o diálogo para selecionar um JSON; ao carregar com sucesso o app mostra uma notificação "Carregado de <arquivo>".
4. Listagem de eventos usa o formato: `Titulo | Dia(s) | Horario | tags` com data no formato `DD-MM-YYYY` para exibição.

---

## Observações de Usabilidade e Compatibilidade

- O parser de `JSONPersistence` aceita datas gravadas tanto no formato interno `YYYY-MM-DD HH:MM` quanto em `DD-MM-YYYY HH:MM` (mais amigável ao usuário). Se um arquivo tiver datas inválidas, o carregamento apresenta uma mensagem de erro.
- Mensagens da interface foram reduzidas: o sistema exibe apenas mensagens de erro e a notificação de sucesso ao carregar arquivos. Mensagens informativas (ex.: "Evento criado") não são mais exibidas em MessageBox para reduzir interrupções.
- `ConsoleView` permanece no repositório como implementação alternativa, mas não é incluído no build por padrão.

---

## Link do Vídeo apresentando Projeto:

Link do Google Drive: **https://drive.google.com/file/d/1HvPP0ZAQ-4PHmPKFi-o-LJM4PriW4UtQ/view?usp=sharing**