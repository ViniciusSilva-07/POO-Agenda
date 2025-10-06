# Agenda POO (GUI)

## Introdução

Este projeto implementa uma Agenda (planner/organizer) em C++ como trabalho da disciplina de Programação Orientada a Objetos. A interface principal é gráfica (Win32 GUI) e a aplicação abre diretamente a janela da GUI ao iniciar. O código segue princípios de POO e usa persistência em JSON.

---

## Funcionalidades Implementadas

- Criar eventos (título, descrição, início, fim e tags) via formulário gráfico.
- Listar eventos ordenados e agrupar por mês (visualização por mês disponível).
- Salvar eventos em arquivo JSON.
- Carregar eventos a partir de arquivo JSON (o carregador aceita datas em `DD-MM-YYYY HH:MM`).
- Suporte a recorrência diária simples (`RecurrenceRuleDaily`).
- Tratamento de exceções (datas inválidas, erros de I/O).

---

## Estrutura Entregue

- Modelo: `User`, `Calendar`, `Event`, `RecurrenceRule`, `RecurrenceRuleDaily`.
- Controle: `Controller`.
- Visualização: `IView`, `GUIView` (principal). `ConsoleView` permanece como referência, mas não é compilado por padrão.
- Persistência: `IPersistence`, `JSONPersistence`.
- Utilitários: `TimeUtils`, `UtilTemplates`.

Outros arquivos relevantes:
- `CMakeLists.txt` — configuração de build.
- `main.cpp` — ponto de entrada da aplicação.
- `Design/README_DESIGN.md` — detalhes da arquitetura e justificativas de design.

---

## Como Executar (Windows)

Usando CMake (PowerShell):

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Debug
& "${PWD}\agenda.exe"
```

Observação: no Windows o executável será `agenda.exe` dentro da pasta `build`. Use o caminho absoluto ou o operador `&` do PowerShell para executar.

---

## Notas de Uso

- A GUI solicita datas no formato `DD-MM-YYYY` para entrada; para exibição a aplicação também usa `DD-MM-YYYY`.
- O botão "Eventos (Mes)" exibe o mês por extenso (sem acentos) e insere uma linha em branco entre meses para facilitar a leitura.
- O carregador de JSON aceita datas tanto no formato interno (`YYYY-MM-DD HH:MM`) quanto em `DD-MM-YYYY HH:MM` (compatibilidade com arquivos gerados manualmente).
- Mensagens foram reduzidas: a aplicação exibe apenas mensagens de erro e a notificação de sucesso ao carregar arquivos.

---

## Observações Técnicas

- A conversão e manipulação de datas é feita em `src/utils/TimeUtils.cpp`.
- A persistência JSON está em `src/persistence/JSONPersistence.cpp`.
- A interface principal está implementada em `src/view/GUIView.cpp`.

---

Para detalhes completos da arquitetura e justificativas de design, consulte `Design/README_DESIGN.md`.