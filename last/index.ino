// Sketch pour piloter la led integree a la Wemos D1 mini depuis le reseau WiFi

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char *ssid = "hp";           // Identifiant du WiFi que vous utilisez
const char *password = "12345678"; // Mot de passe WiFi que vous utilisez
ESP8266WebServer server(80);       // On demande un serveur qui ecoute sur le port 80

#define DHTPIN 13          // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11     // DHT 11
float t;
float h;

DHT dht(DHTPIN, DHTTYPE);

int led = 2; // La diode LED intégrée à la carte est branchée sur la broche GPIO2 (D4)
String etatGPIO2 = "ON";

// Ecrire une page Web HTML
String WebPage()
{
    String page = R"(
<!DOCTYPE html>
<html lang="fr-FR">
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>SKETCH DEMO ESP8266 Web Server</title>
    <style>
      @import url("https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,600;0,700;0,800;0,900;1,600;1,700;1,800&display=swap");
      * {
        padding: 0;
        margin: 0;
        box-sizing: border-box;
        font-family: "Poppins", Arial, Helvetica, Sans-Serif;
      }
      a {
        color: inherit;
      }
      body {
        overflow-x: hidden;
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
      input[type="submit"] {
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
      #off_btn,
      #on_btn {
        padding: 11px;
        cursor: pointer;
        outline: none;
        border: none;
        color: inherit;
      }

      #off_btn {
        background: red;
      }
      #on_btn {
        background: green;
      }

      #on_btn:disabled,
      #off_btn:disabled {
        opacity: 0.4;
      }

      #pop_out_temp {
        position: absolute;
        bottom: 40px;
        background: rgba(0, 255, 255, 0.49);
        padding: 8px;
        padding-left: 30px;
        width: 260px;
        /* font-style: italic; */
        font-size: small;
        height: 100px;
        border-radius: 10px 0 0 10px;
        display: flex;
        flex-direction: column;
        justify-content: space-evenly;
        animation: moveLeft 1.4s ease-in-out forwards;
      }

      @keyframes moveLeft {
        0% {
          right: -260px;
        }
        100% {
          right: 0;
        }
      }

      #pop_out_temp svg {
        width: 15px;
        height: 15px;
        fill: orange;
      }

      #pop_out_temp > div {
        color: orange;
        text-transform: uppercase;
        display: flex;
        align-items: center;
        gap: 2px;
      }

      #pop_out_temp .svg {
        display: flex;
        align-items: center;
        justify-content: center;
      }

      #humidity,
      #temperature {
        color: #fff;
      }

      #temperature::after {
        content: "°";
      }

      #humidity::after {
        content: "%";
      }
    </style>
    <!-- Load the Firebase SDKs -->

    <script type="module">
      import { initializeApp } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-app.js";
      import {
        getAuth,
        onAuthStateChanged,
        signInWithEmailAndPassword,
        signOut,
      } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-auth.js";

      // TODO: Add SDKs for Firebase products that you want to use

      // Your web app's Firebase configuration
      const firebaseConfig = {
        apiKey: "AIzaSyC0bNbWy1Cl34ejQEPKOwrL5XWRXGtpN50",
        authDomain: "mini-project-59d59.firebaseapp.com",
        databaseURL: "https://mini-project-59d59-default-rtdb.firebaseio.com",
        projectId: "mini-project-59d59",
        storageBucket: "mini-project-59d59.appspot.com",
        messagingSenderId: "1031167507444",
        appId: "1:1031167507444:web:a2edb6701b1e947c4197a1",
        measurementId: "G-BVVP0PEWHH",
      };

      // Initialize Firebase
      const app = initializeApp(firebaseConfig);
      const auth = getAuth(app);

      const loginForm = document.getElementById("login-form");
      const submit_login_btn = document.getElementById("submit_login");
      const message = document.getElementById("message");

      loginForm.addEventListener("submit", (e) => {
        e.preventDefault();
        submit_login_btn.value = "Login...";
        submit_login_btn.disabled = true;
        // submit_login_btn.value = 'login...';
        // submit_login_btn.disabled = true;
        message.value = "You are logged in";
        const email = loginForm.email.value;
        const password = loginForm.password.value;

        signInWithEmailAndPassword(auth, email, password)
          .then((userCredential) => {
            // User signed in successfully...
            const user = userCredential.user;
            console.log("Logged in user:", user);
            submit_login_btn.value = "Login";
            submit_login_btn.disabled = false;
          })
          .catch((error) => {
            // Handle login errors...
            console.error("Login failed:", error);
            message.innerText = "invalid email or password";
            submit_login_btn.value = "Login";
            submit_login_btn.disabled = false;
          });
      });
      const loader = document.getElementById("loader");
      const secured_page = document.getElementById("secured_page");
      const unsecured_page = document.getElementById("unsecured_page");
      const user_email_div = document.getElementById("user-email");

      onAuthStateChanged(auth, (user) => {
        if (user) {
          // User is logged in, show the protected form
          loader.style.display = "none";
          secured_page.style.display = "block";
          unsecured_page.style.display = "none";
          var userEmail = user.email;
          user_email_div.innerHTML = userEmail;
        } else {
          // User is not logged in, hide the protected form
          loader.style.display = "none";
          secured_page.style.display = "none";
          unsecured_page.style.display = "block";
        }
      });

      // Handle logout button click
      const logoutButton = document.getElementById("logoutButton");

      logoutButton.addEventListener("click", () => {
        signOut(auth);
      });
    </script>
  </head>
  <body onload='get_temp()'>
  <div id="pop_out_temp">
      <div>
        <span class="svg">
          <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 384 512">
            <path
              d="M192 512C86 512 0 426 0 320C0 228.8 130.2 57.7 166.6 11.7C172.6 4.2 181.5 0 191.1 0h1.8c9.6 0 18.5 4.2 24.5 11.7C253.8 57.7 384 228.8 384 320c0 106-86 192-192 192zM96 336c0-8.8-7.2-16-16-16s-16 7.2-16 16c0 61.9 50.1 112 112 112c8.8 0 16-7.2 16-16s-7.2-16-16-16c-44.2 0-80-35.8-80-80z"
            />
          </svg>
        </span>
        <span>temperature : </span>
        <span id="temperature">34</span>
      </div>
      <div>
        <span class="svg">
          <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
            <path
              d="M448 96a32 32 0 1 0 -64 0 32 32 0 1 0 64 0zM320 96a96 96 0 1 1 192 0A96 96 0 1 1 320 96zM144 64c-26.5 0-48 21.5-48 48V276.5c0 17.3-7.1 31.9-15.3 42.5C70.2 332.6 64 349.5 64 368c0 44.2 35.8 80 80 80s80-35.8 80-80c0-18.5-6.2-35.4-16.7-48.9c-8.2-10.6-15.3-25.2-15.3-42.5V112c0-26.5-21.5-48-48-48zM32 112C32 50.2 82.1 0 144 0s112 50.1 112 112V276.5c0 .1 .1 .3 .2 .6c.2 .6 .8 1.6 1.7 2.8c18.9 24.4 30.1 55 30.1 88.1c0 79.5-64.5 144-144 144S0 447.5 0 368c0-33.2 11.2-63.8 30.1-88.1c.9-1.2 1.5-2.2 1.7-2.8c.1-.3 .2-.5 .2-.6V112zM192 368c0 26.5-21.5 48-48 48s-48-21.5-48-48c0-20.9 13.4-38.7 32-45.3V272c0-8.8 7.2-16 16-16s16 7.2 16 16v50.7c18.6 6.6 32 24.4 32 45.3z"
            />
          </svg>
        </span>
        <span>humidity : </span>
        <span id="humidity">78</span>
      </div>
    </div>
    <div id="loader">
      <h2>connecting to <span class="color-orange">firebase</span></h2>
      <div class="lds-facebook">
        <div></div>
        <div></div>
        <div></div>
      </div>
    </div>
    <div id="secured_page">
      <h1>ISET CHARGUIA</h1>
      <div id="user-content">
        <div>
          user :
          <span id="user-email"></span>
        </div>
        <button id="logoutButton">logout</button>
      </div>
      <form method="GET" id="myForm" name="myForm">
        <div id="command-etat">
          <h3>Commande d'une LED sur GPIO</h3>
          <div id="etat">GPIO2 : <span id="led_state">)";
          page += etatGPIO2;
          page += R"(</span></div>
        </div>
        <button id="on_btn" type="button" onClick='toggle_led(1)'>
          <span> ON</span>
        </button>
        <button disabled id="off_btn" type="button" onClick='toggle_led(0)'>
          <span>OFF</span>
        </button>
      </form>
      <br /><br />
      <p id="iset_ch"><a href="#"> ISET Charguia &copy;</a></p>
    </div>
    <div id="unsecured_page">
      <form method="POST" id="login-form">
        <h1 class="title">Login</h1>
        <div id="message"></div>
        <label for="email">Email</label>
        <input type="email" name="email" id="email" required />
        <label for="password">Password</label>
        <input type="password" name="password" id="password" required />
        <input type="submit" value="login" id="submit_login" />
      </form>
    </div>
  </body>
  <script>
    async function toggle_led(value) {
      const url = window.location.hostname + `/?GPIO2=${value}`;
      const response = await fetch(url);
      const data = await response.json();
      document.getElementById("on_btn").disabled = value == 1;
      document.getElementById("off_btn").disabled = value == 0;
      document.getElementById("led_state").innerHTML =
        value == 1 ? "ON" : "OFF";
    }

    async function get_temp() {
      const url = window.location.hostname + "/temp";
      try {
        const response = fetch(url);
        const data = await response.json();
        document.getElementById("temperature").innerHTML = data.temperature;
        document.getElementById("humidity").innerHTML = data.humidity;
      } catch (error) {
        console.log({ error });
      }
    }

    setInterval(get_temp, 10000);
  </script>
</html>
)";

    return page;
}

void Racine()
{
    if (server.hasArg("GPIO2"))
    {
        handle_toogle_led();
    }
    else
    {
        server.send(200, "text/html", WebPage());
    }
}

void handle_toogle_led()
{
    // Actualise le GPIO
    String GPIOValue;
    GPIOValue = server.arg("GPIO2");
    Serial.print("Set GPIO to ");
    Serial.println(GPIOValue);
    if (GPIOValue == "1")
    {
        digitalWrite(2, 0);
        etatGPIO2 = "ON";
    }
    else if (GPIOValue == "0")
    {
        digitalWrite(2, 1);
        etatGPIO2 = "OFF";
    }
    else
    {
        Serial.println("Erreur Valeur LED");
    }
    String jsonResponse = "{\"state\":" + String(GPIOValue) + "}";
    server.send(200, "application/json", jsonResponse);
}

void handle_temp(){
   h = dht.readHumidity();
   t = dht.readTemperature();

	String jsonResponse = "{\"temperature\": " + String(t) + ", \"humidity\": " + String(h) + "}";

	server.send(200, "application/json", jsonResponse);
}

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");

    // on attend d'etre connecté au WiFi avant de continuer
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // on affiche l'adresse IP qui nous a ete attribuée
    Serial.println("");
    Serial.print("Addresse IP : ");
    Serial.println(WiFi.localIP());
    // on definit les points d'entree (les URL a saisir dans le navigateur web) et on affiche un simple texte
    server.on("/", Racine);
    server.on("/temp", handle_temp);

    // on demarre le serveur web
    server.begin();
    pinMode(led, OUTPUT); // Initialise la broche GPIO2 comme une sortie
}

// Cette boucle s'exécute à l'infini
void loop()
{
    // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
    server.handleClient();
}
