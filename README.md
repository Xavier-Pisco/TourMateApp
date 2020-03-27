# TourMateApp: rotas turísticas urbanas adaptáveis
Gráfico UML aqui: https://drive.google.com/open?id=1ihLMK_uBxhjP6TgMdNwc7WoXynCyTxIS

Relatório aqui: https://docs.google.com/document/d/1Ep8xADx7tTjfCKDWgqF5iQM1fDkiLiRRzuxPnfa3NhU/edit?fbclid=IwAR2BpMNGTk59qejCJQWD0TQ5J5XuWEMoSQlBdEFn7-O26Jw7-OuukqtLfqw

Seja em passeio ou em trabalho, turistas terão diferentes interesses e disponibilidades para visitar as atrações turísticas de uma cidade. Em viagens curtas de trabalho, pessoas podem ter o interesse de realizar visitas de muito curta duração, tendo de identificar circuitos mais curtos que lhes levem aos pontos de interesse mais importantes, segundo as suas preferências. Turistas em viagem de lazer já terão potencialmente flexibilidade de horários e maior disponibilidade para itinerários mais longos, incluindo muito mais pontos de atração.

![Map](https://lh6.googleusercontent.com/UeohVp4z2GoahivuGTglVe8Yki2LuXQtipszeGzLEKz45a-LWVBmc211D4dSd4mmzzHCOlGOenttn-VVBR6zZFnhxQ7yFmNEGmPwqDeNUeEnY26Z9xgvowlFWHAfyJhOET-JfyMk "Map")

Nete projeto, pretende-se implementar a aplicação TourMateApp, que permite a construção de itinerários turísticos adaptáveis às preferências e disponibilidades do utilizador. A aplicação deve manter uma lista dos pontos de interesse (POI) turísticos, e ser capaz de sugerir itinerários que incluem as atrações mais adequadas ao perfil do utilizador, num caminho capaz de ser realizado no tempo indicado, desde uma origem até um destino final, indicados pelo utilizador. Por exemplo, alguém e viagem de trabalho poderá desejar realizar uma caminhada de 30 minutos, no seu intervalo, que inclua os pontos turísticos próximos do local onde está a ter reuniões. Ou, um turista poderá desejar fazer um circuito a passar pelos principais museus da cidade, em numa visita de dia inteiro. As recomendações deverão maximizar o número de atrações turísticas de acordo com o perfil e preferências do utilizador, que poderá também selecionar circuitos a pé, de carro ou de transporte público. A aplicação deve também receber como input a descrição da rede da cidade e respectivas localizações dos POIs disponíveis,

Avalie a conectividade do grafo, a fim de evitar que POIs sejam escolhidos em em pontos inacessíveis da rede. Algumas vezes, obras nas vias públicas podem fazer com que certas zonas tornem-se inacessíveis, tornando os caminhos inviáveis.

Considere a possibilidade de utilizar mapas reais, extraídos do OpenStreetMaps (www.openstreetmap.org) e coordenadas geográficas de alguns pontos de interesse turísticos.
