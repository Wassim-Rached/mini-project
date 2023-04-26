// Sketch pour piloter la led integree a la Wemos D1 mini depuis le reseau WiFi

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid = "WASSIM 3090";          // Identifiant du WiFi que vous utilisez
const char* password = "03sX078(";  // Mot de passe WiFi que vous utilisez
ESP8266WebServer server(80);        // On demande un serveur qui ecoute sur le port 80

int led = 2; // La diode LED intégrée à la carte est branchée sur la broche GPIO2 (D4)
String  etatGPIO2 = "ON";


//Ecrire une page Web HTML
String WebPage(){
  String page =R"(
<!DOCTYPE html>
<html lang='fr-FR'>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1.0' />
    <!-- <meta http-equiv='refresh' content='10' /> -->
    <title>SKETCH DEMO ESP8266 Web Server</title>
    <style>
      @import url('https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,600;0,700;0,800;0,900;1,600;1,700;1,800&display=swap');
      * {
        padding: 0;
        margin: 0;
        box-sizing: border-box;
        font-family: 'Poppins', Arial, Helvetica, Sans-Serif;
      }
      a {
        color: inherit;
      }
      body {
        min-height: 100vh;
        background-color: #333;
        color: white;
      }
      button {
        border-radius: 4px;
        outline: none;
        border: 1px solid orange;
        cursor: pointer;
        background-color: #333;
        color: orange;
        padding: 9px 14px;
      }
      #secured_page {
        display: none;
      }
      #secured_page > h1 {
        color: orange;
        margin-bottom: 60px;
        text-align: center;
      }
      #secured_page,
      #unsecured_page {
        max-width: 900px;
        padding: 20px;
        margin: auto;
      }
      #unsecured_page {
        margin-top: 60px;
      }
      form {
        display: flex;
        flex-direction: column;
        gap: 10px;
      }
      input {
        padding: 8px;
      }
      input[type='submit'] {
        cursor: pointer;
      }
      #user-content {
        align-items: center;
        justify-content: space-between;
        display: flex;
        gap: 7px;
      }
      #loader {
        position: absolute;
        left: 0;
        top: 0;
        right: 0;
        bottom: 0;
        background-color: #333;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        display: flex;
      }
      .lds-facebook {
        display: inline-block;
        position: relative;
        width: 80px;
        height: 80px;
      }
      .lds-facebook div {
        display: inline-block;
        position: absolute;
        left: 8px;
        width: 16px;
        background: #fff;
        animation: lds-facebook 1.2s cubic-bezier(0, 0.5, 0.5, 1) infinite;
      }
      .lds-facebook div:nth-child(1) {
        left: 8px;
        animation-delay: -0.24s;
      }
      .lds-facebook div:nth-child(2) {
        left: 32px;
        animation-delay: -0.12s;
      }
      .lds-facebook div:nth-child(3) {
        left: 56px;
        animation-delay: 0;
      }
      @keyframes lds-facebook {
        0% {
          top: 8px;
          height: 64px;
        }
        50%,
        100% {
          top: 24px;
          height: 32px;
        }
      }
      #iset_ch {
        color: gray;
        font-style: italic;
        position: absolute;
        bottom: 6px;
        left: 0;
        right: 0;

        text-align: center;
      }
      #user-email {
        color: rgb(0, 159, 0);
      }
      #myForm {
        margin-top: 20px;
        padding-left: 20px;
      }
      #command-etat {
        display: flex;
        gap: 10px;
        align-items: center;
        justify-content: space-between;
      }
      .title {
        margin-bottom: 20px;
        font-size: 2.6rem;
        text-transform: uppercase;
        text-align: center;
        color: orange;
      }
      #message {
        height: 20px;
        text-align: center;
        color: rgb(235, 46, 46);
      }
      #etat {
        width: 100px;
      }
      .color-orange {
        color: orange;
      }
    </style>
    <!-- Load the Firebase SDKs -->

    <script type='module'>
      import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.17.2/firebase-app.js';
      import {
        getAuth,
        onAuthStateChanged,
        signInWithEmailAndPassword,
        signOut,
      } from 'https://www.gstatic.com/firebasejs/9.17.2/firebase-auth.js';

      // TODO: Add SDKs for Firebase products that you want to use

      // Your web app's Firebase configuration
      const firebaseConfig = {
        apiKey: 'AIzaSyC0bNbWy1Cl34ejQEPKOwrL5XWRXGtpN50',
        authDomain: 'mini-project-59d59.firebaseapp.com',
        databaseURL: 'https://mini-project-59d59-default-rtdb.firebaseio.com',
        projectId: 'mini-project-59d59',
        storageBucket: 'mini-project-59d59.appspot.com',
        messagingSenderId: '1031167507444',
        appId: '1:1031167507444:web:a2edb6701b1e947c4197a1',
        measurementId: 'G-BVVP0PEWHH',
      };

      // Initialize Firebase
      const app = initializeApp(firebaseConfig);
      const auth = getAuth(app);

      const loginForm = document.getElementById('login-form');
      const submit_login_btn = document.getElementById('submit_login');
      const message = document.getElementById('message');

      loginForm.addEventListener('submit', (e) => {
        e.preventDefault();
        submit_login_btn.value = 'Login...';
        submit_login_btn.disabled = true;
        // submit_login_btn.value = 'login...';
        // submit_login_btn.disabled = true;
        message.value = 'You are logged in';
        const email = loginForm.email.value;
        const password = loginForm.password.value;

        signInWithEmailAndPassword(auth, email, password)
          .then((userCredential) => {
            // User signed in successfully...
            const user = userCredential.user;
            console.log('Logged in user:', user);
            submit_login_btn.value = 'Login';
            submit_login_btn.disabled = false;
          })
          .catch((error) => {
            // Handle login errors...
            console.error('Login failed:', error);
            message.innerText = 'invalid email or password';
            submit_login_btn.value = 'Login';
            submit_login_btn.disabled = false;
          });
      });
      const loader = document.getElementById('loader');
      const secured_page = document.getElementById('secured_page');
      const unsecured_page = document.getElementById('unsecured_page');
      const user_email_div = document.getElementById('user-email');

      onAuthStateChanged(auth, (user) => {
        if (user) {
          // User is logged in, show the protected form
          loader.style.display = 'none';
          secured_page.style.display = 'block';
          unsecured_page.style.display = 'none';
          var userEmail = user.email;
          user_email_div.innerHTML = userEmail;
        } else {
          // User is not logged in, hide the protected form
          loader.style.display = 'none';
          secured_page.style.display = 'none';
          unsecured_page.style.display = 'block';
        }
      });

      // Handle logout button click
      const logoutButton = document.getElementById('logoutButton');

      logoutButton.addEventListener('click', () => {
        signOut(auth);
      });
      const myForm = document.getElementById('myForm');
      const radioButtons = document.querySelectorAll('input[type=\"radio\"]');
      radioButtons.forEach((radioButton) => {
        radioButton.addEventListener('change', () => {
          myForm.submit();
        });
      });
      myForm.onsubmit((e) => {
        e.preventDefault();
      });
    </script>
  </head>
  <body>
    <div id='loader'>
      <h2>connecting to <span class='color-orange'>firebase</span></h2>
      <div class='lds-facebook'>
        <div></div>
        <div></div>
        <div></div>
      </div>
    </div>
    <div id='secured_page'>
      <h1>ISET CHARGUIA</h1>
      <div id='user-content'>
        <div>
          user :
          <span id='user-email'></span>
        </div>
        <button id='logoutButton'>logout</button>
      </div>
      <form method='GET' id='myForm' name='myForm'>
        <div id='command-etat'>
          <h3>Commande d'une LED sur GPIO</h3>
          <div id='etat'>GPIO2 : <span>)";
            page+=etatGPIO2;
            page+=R"(
              </span></div>
        </div>
        <label>
          <input type='radio' name='GPIO2' value='1' id='ON' 
          )";
          if (etatGPIO2 == "ON"){
          page+="checked";
          }
          page+=R"(
          />
          <span>ON</span>
        </label>
        <label>
          <input type='radio' name='GPIO2' value='0')";
          if (etatGPIO2 == "OFF"){
          page+="checked";
          }
          page+=R"(
          />
          <span> OFF</span>
        </label>
      </form>
      <br /><br />
      <p id='iset_ch'><a href='#'>ISET Charguia &copy;</a></p>
    </div>
    <div id='unsecured_page'>
      <form method='POST' id='login-form'>
        <h1 class='title'>Login</h1>
        <div id='message'></div>
        <label for='email'>Email</label>
        <input type='email' name='email' id='email' required />
        <label for='password'>Password</label>
        <input type='password' name='password' id='password' required />
        <input type='submit' value='login' id='submit_login' />
      </form>
    </div>
  </body>
</html>)";

return page;
}

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
