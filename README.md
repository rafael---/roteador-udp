# Roteador UDP
## Implementação de uma variante do protocolo <a href="https://en.wikipedia.org/wiki/Routing_Information_Protocol">RIP</a>

### Instruções de uso:
#### Compilação:

A compilação é feita através de um Makefile. Basta e executar o comando <em>make</em>

#### Inicialização:
  
É necessário entrar no diretório bin/.

Para inicializar o roteador basta passar o identificador (ID) como parâmetro.

Exemplos:

    ./server 1
    ./server 2

Obs.: Os roteadores são enumerados de 1 a &lt;n&gt;, sendo 0 um roteador inválido

#### Utilização:

O comando help mostra este menu de ajuda.

O comando status informa qual é o estado do roteador desejado.
Caso o roteador seja vizinho, ele mostrará os status OK ou OFFLINE.
Caso contrário, será mostrado o status DESCONHECIDO.

Formato: 
    
    status <id>
Exemplo:

    status 2

O comando send envia uma mensagem para o roteador destino desejado
Formato:
    
    send <id> mensagem

Exemplo:

    send 1 Olá!
    send 2 Hello router!
    
As mensagens podem ter até 255 caracteres, incluindo espaços


#### Configuração

O programa lê o arquivo roteador.config para efetuar a configuração dos roteadores.

Exemplo:

    1 25001 127.0.0.1
    2 25002 127.0.0.1
    3 25003 127.0.0.1
    4 25004 127.0.0.1
    5 25005 127.0.0.1
    6 25006 127.0.0.1

E um arquivo enlaces.config para conhecer os enlaces da rede, com cada linha contendo a origem, destino e o custo.

Exemplo:

    1       2      10
    1       3      15
    2       4      2
    2       5      5
    3       4      2
    4       6      10
    5       6      5


#### Constantes do programa (Arquivo defs.h)

DEBUG é uma constante para depuração, quando for definida mostrará mensagens mais detalhadas e status do sistema

Exemplo:

    #define DEBUG 1

INTERVALO é o intervalo de atualização dos roteadores, em segundos. O padrão é 1

TIMEOUT é o tempo de expiração de uma mensagem, após esse tempo a mensagem é reenviada caso não receba uma confirmação.
  O TIMEOUT padrão é 5 segundos
  
MAX é a distância máxima que um roteador pode ter de outro. O padrão é 15

MAX_NODES é o número máximo de roteadores permitidos. O padrão é 7, mas pode ser alterado. Não é aconselhável utilizar um
  número maior, pois o vetor-distância é enviado como char e tem o número máximo permitido 127. Logo, MAX*MAX_NODES deve ser
  menor que 127.
  
TENTATIVAS é o número de tentativas de reenvio de uma mensagem. O padrão é 3


#### Funcionamento do prococolo

Os roteadores enviam periodicamente informações de seus vizinhos e seus respectivos custos para os outros vizinhos.
Caso o roteador não envie informações por mais de 4 * INTERVALO segundos, ele é considerado OFFLINE
