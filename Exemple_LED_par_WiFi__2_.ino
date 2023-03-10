// Sketch pour piloter la led integree a la Wemos D1 mini depuis le reseau WiFi

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid = "DESKTOP-J7N4R0M 0988";          // Identifiant du WiFi que vous utilisez
const char* password = "80D[860i";  // Mot de passe WiFi que vous utilisez
ESP8266WebServer server(80);        // On demande un serveur qui ecoute sur le port 80

int led = 2; // La diode LED intégrée à la carte est branchée sur la broche GPIO2 (D4)
String  etatGPIO2 = "ON";


//Ecrire une page Web HTML
String WebPage(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";   // Un code HTML actualise la page automatiquement    
  page += "<title>SKETCH DEMO ESP8266 Web Server</title>";               // Titre de la barre du navigateur
  page += "<style> body { background-color: red; font-family: Arial, Helvetica, Sans-Serif; Color: white; }</style>"; // style de la page
  page += "</head><body><h1>ISET CHARGUIA</h1>";
  page += "</head><body><h1></h1>";
  page += "<h3>Commande d'une LED sur GPIO</h3>";                                  // Sous-titre (H3)
  page += "<form action='/' method='POST' id='myForm'>";                // Le formulaire sera envoye avec une requete de type POST 
  page += "<ul><li>GPIO2 (etat: ";                             // Première ligne de la liste (ul) avec D0
  page += etatGPIO2;                                           // on concatene la chaine contenant l'etat de la sortie
  page += ") ";
  page += " <INPUT type='radio' name='GPIO2' value='1' onchange='submitForm()'>ON";     // Bouton pour activer D0
  page += "<INPUT type='radio' name='GPIO2' value='0' onchange='submitForm()'>OFF</li></ul>"; // et le desactiver
  page += "</head><body><h1></h1>";
  page +="<script>";
  page +="function submitForm() {";
  page +="  document.getElementById('myForm').submit();}";
  page +="</script>";
  page += "<br><br><p><a hrf=''>(c)ISET Charguia</p>";
  page += "</body></html>";
  return page;
}

/*
//page login
String Login(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
page += "<head> <title>Page de Login</title></head>";
page += "<body>";
page += "	<h2>Connexion</h2>";
page += "<form action='/' method='POST'>";
page += "<label for="username">Nom d'utilisateur:</label>";
page += "<input type="text" id="username" name="username" placeholder="username"><br><br>";
page += "<label for="password">Mot de passe:</label>";
page += "<input type="password" id="password" name="password" placeholder="password"><br><br>";
page += "<input type="submit" value="Se connecter">";
page += "</form>";
page += "</body>";
page += "</html>";
return page;
}
*/

void Racine(){ 
  if ( server.hasArg("GPIO2") ) {
    Soumettre();
  } else {
    server.send ( 200, "text/html", WebPage() );
  }  
}

void Soumettre() {
  // Actualise le GPIO 
  String GPIOValue;
  GPIOValue = server.arg("GPIO2");
  Serial.print("Set GPIO to "); Serial.println(GPIOValue);
  if ( GPIOValue == "1" ) {
    digitalWrite(2, 0);
    etatGPIO2 = "ON";
    server.send ( 200, "text/html", WebPage() );
  } else if ( GPIOValue == "0" ) {
    digitalWrite(2, 1);
    etatGPIO2 = "OFF";
    server.send ( 200, "text/html", WebPage() );
  } else {
    Serial.println("Erreur Valeur LED");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  
  // on attend d'etre connecté au WiFi avant de continuer
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  // on affiche l'adresse IP qui nous a ete attribuée
  Serial.println("");
  Serial.print("Addresse IP : ");
  Serial.println(WiFi.localIP());
  // on definit les points d'entree (les URL a saisir dans le navigateur web) et on affiche un simple texte
  server.on ( "/", Racine );

  // on demarre le serveur web
  server.begin();
  pinMode(led, OUTPUT);     // Initialise la broche GPIO2 comme une sortie
}

// Cette boucle s'exécute à l'infini
void loop() {
    // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
  server.handleClient();
}
