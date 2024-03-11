#include <WString.h>

String armaPaginaPanelLampara( String nomLampara, int statusTunelManual, int statusSuperficiesManual, int statusTunelAuto, int statusSuperficiesAuto ){
    String page = PSTR(
  "<html>"
  "<head>"
    "<meta charset=\"UTF-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">"
    //"<link href=\"https://fonts.googleapis.com/css?family=Roboto:100,100i,300,300i,400,400i,500,500i,700,700i,900,900i\" rel=\"stylesheet\">"
    //"<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">"
    "<style type=\"text/css\">"

    "* *{"
      "margin: 0;"
      "padding: 0;"
      "box-sizing: border-box;"
      "font-family: 'Roboto', sans-serif;"
    "}"
    
    "body{"
      "background: #f1f1f1;"
    "}" 
    ".menu > a:link {"
    "position: absolute;"
    "display: inline-block;"
    "right: 12px;"
    "padding: 0 6px;"
    "text-decoration: none;"
    "}"
    ".container{"
      "width: 100%;"
      "height: auto;"
      "padding: 5px;"
      "display: flex;"
      "justify-content: center;"
    "}"
    ".nodo-master-container{"
      "display: flex;"
      "width: auto;"
      "height: auto;"
      "background:#fff;"
      "box-shadow: 0 1px 1px 0 rgba(0, 0, 0, .2);"
    "}"
    ".register{"
      "padding: 10px 10px 0px 10px;"
      "position: relative;"
    "}"
    ".register h2, .register h3, .register h4 {"
      "color: #6a5acd;"
      "margin-top: 15px;"
      "margin-bottom: 10px;"
      "text-align: center;"
    "}"
    ".register h1{"
      "color: #66708a;"
      "font-size: 30px;"
      "margin-top: 10px;"
      "margin-bottom: 10px;"
      "text-align: center;"
    "}"
    ".sub_h3{"
      "color: #3e5f8a !important;"
      "font-size: 18px;"
      "margin-top: 10px;"
      "margin-bottom: 10px;"
      "text-align: center;"
    "}"

    ".mid {"
      "display: flex;"
      "align-items: center;"
      "justify-content: center;"
      /* "padding-top:1em;" */
      "margin-bottom: 10px;"
    "}"

    /* Switch starts here */
    ".rocker {"
      "display: inline-block;"
      "position: relative;"
      /*
      SIZE OF SWITCH
      ==============
      All sizes are in em - therefore
      changing the font-size here
      will change the size of the switch.
      See .rocker-small below as example.
      */
      "font-size: 1.4em;"
      "font-weight: bold;"
      "text-align: center;"
      "text-transform: uppercase;"
      "color: #888;"
      "width: 7em;"
      "height: 4em;"
      "overflow: hidden;"
      "border-bottom: 0.5em solid #eee;"
    "}"

    ".rocker::before {"
      "content: \"\";"
      "position: absolute;"
      "top: 0.5em;"
      "left: 0;"
      "right: 0;"
      "bottom: 0;"
      "background-color: #999;"
      "border: 0.5em solid #eee;"
      "border-bottom: 0;"
    "}"

    ".rocker input {"
      "opacity: 0;"
      "width: 0;"
      "height: 0;"
    "}"

    ".switch-left,"
    ".switch-right {"
      "cursor: pointer;"
      "position: absolute;"
      "display: flex;"
      "align-items: center;"
      "justify-content: center;"
      "height: 2.5em;"
      "width: 3em;"
      "transition: 0.2s;"
    "}"

    ".switch-left {"
      "height: 2.4em;"
      "width: 2.75em;"
      "left: 0.85em;"
      "bottom: 0.4em;"
      "background-color: #ddd;"
      "transform: rotate(15deg) skewX(15deg);"
    "}"

    ".switch-right {"
      "right: 0.5em;"
      "bottom: 0;"
      "background-color: #F67280;"
      "color: #fff;"
    "}"

    ".switch-left::before,"
    ".switch-right::before {"
      "content: "";"
      "position: absolute;"
      "width: 0.4em;"
      "height: 2.45em;"
      "bottom: -0.45em;"
      "background-color: #ccc;"
      "transform: skewY(-65deg);"
    "}"

    ".switch-left::before {"
      "left: -0.4em;"
    "}"

    ".switch-right::before {"
      "right: -0.375em;"
      "background-color: transparent;"
      "transform: skewY(65deg);"
    "}"

    "input:checked + .switch-left {"
      "background-color: #355C7D;"
      "color: #fff;"
      "bottom: 0px;"
      "left: 0.5em;"
      "height: 2.5em;"
      "width: 3em;"
      "transform: rotate(0deg) skewX(0deg);"
    "}"

    "input:checked + .switch-left::before {"
      "background-color: transparent;"
      "width: 3.0833em;"
    "}"

    "input:checked + .switch-left + .switch-right {"
      "background-color: #ddd;"
      "color: #888;"
      "bottom: 0.4em;"
      "right: 0.8em;"
      "height: 2.4em;"
      "width: 2.75em;"
      "transform: rotate(-15deg) skewX(-15deg);"
    "}"

    "input:checked + .switch-left + .switch-right::before {"
      "background-color: #ccc;"
    "}"

    /* Keyboard Users */
    "input:focus + .switch-left {"
      "color: #333;"
    "}"

    "input:checked:focus + .switch-left {"
      "color: #fff;"
    "}"

    "input:focus + .switch-left + .switch-right {"
      "color: #fff;"
    "}"

    "input:checked:focus + .switch-left + .switch-right {"
      "color: #333;"
    "}"

    ".onoffswitch {"
        "position: relative;" 
        "width: 86px;"
        "margin-top: 10px;"
        "margin-left: auto !important;"
        "margin-right: auto !important;"
        "margin-bottom: 10px;"
        "-webkit-user-select:none; -moz-user-select:none; -ms-user-select: none;"
    "}"
    ".onoffswitch-checkbox {"
        "position: absolute;"
        "opacity: 0;"
        "pointer-events: none;"
    "}"
    ".onoffswitch-label {"
        "display: block; overflow: hidden; cursor: pointer;"
        "border: 2px solid #999999; border-radius: 50px;"
    "}"
    ".onoffswitch-inner {"
       "display: block; width: 200%; margin-left: -100%;"
        "transition: margin 0.3s ease-in 0s;"
    "}"
    ".onoffswitch-inner:before, .onoffswitch-inner:after {"
        "display: block; float: left; width: 50%; height: 24px; padding: 0; line-height: 24px;"
        "font-size: 18px; color: white; font-family: Trebuchet, Arial, sans-serif; font-weight: bold;"
        "box-sizing: border-box;"
    "}"
    ".onoffswitch-inner:before {"
        "content: \"ON\";"
        "padding-left: 12px;"
        "background-color: #276678; color: #FFFFFF;"
    "}"
    ".onoffswitch-inner:after {"
        "content: \"OFF\";"
        "padding-right: 12px;"
        "background-color: #EEEEEE; color: #999999;"
        "text-align: right;"
    "}"
    ".onoffswitch-switch {"
        "display: block; width: 31px; margin: -3.5px;"
        "background: #FFFFFF;"
        "position: absolute; top: 0; bottom: 0;"
        "right: 58px;"
        "border: 2px solid #999999; border-radius: 50px;"
        "transition: all 0.3s ease-in 0s;" 
    "}"
    ".onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-inner {"
        "margin-left: 0;"
    "}"
    ".onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-switch {"
        "right: 0px;"
    "}"

    ".button_config {"
        "background-color: #355C7D;"
        "color: white;" 
        "border: 4px solid #355C7D;"
        "padding: 15px 32px;"
        "text-align: center;"
        "text-decoration: none;"
        "display: inline-block;"
        "font-size: 16px;"
        "margin: 4px 2px;"
        "cursor: pointer;"
    "}"

    ".button_config:hover {"
        "background-color: white;"
        "color: #355C7D;"
    "}"

    ".botonLogout {"
      "width: 40%;"
      "height: 6vh;"
      "padding: 10px;"
      "background: #762900!important;"
      "color: #fff!important;"
      "font-size: 14px;"
      "margin-top: 30px;"
      "margin-bottom: 10px;"
      "box-shadow: 0 0 3px 0 rgba(0, 0, 0, .2);"
      "cursor: pointer;"
      "border: none;"
    "}"
    
    ".formulario_label {"
      "display: block;" 
      "margin-bottom: 20px;"
      "font-size: 14px;"
      "font-weight: bold;"
      "text-align: center;"
      //"color: #6a5acd;"
    "}"

    "</style>"

    "<script src=\"https://code.jquery.com/jquery-3.5.1.js\" integrity=\"sha256-QWo7LDvxbWT2tbbQ97B53yJnYU3WhH/C8ycbRAkjPDc=\" crossorigin=\"anonymous\" type=\"text/javascript\"></script>"
    "<script>"
      "function updateSwitchesUI(encRes) {"
        "var switch1 = document.getElementById('switch1');"
        "var switch2 = document.getElementById('switch2');"
        /* "console.log(\"RES: \" + encRes);" */

        "var stTunel = encRes.substring(0,1);"
        "var stSup = encRes.substring(2,3);"

        /* "console.log(\"TUNEL: \" + stTunel);"
        "console.log(\"SUPERFICIE: \" + stSup);" */

        /* "switch1.value = encTunel;" */
        "if(stTunel === \"1\") {"
          "switch1.checked = true;" 
        "} else {"
          "switch1.checked = false;"
        "}"

        "if(stSup === \"1\") {"
          "switch2.checked = true;" 
        "} else {"
          "switch2.checked = false;"
        "}"
      "}"

      "function ajaxCall() {"
          "var xmlhttp = new XMLHttpRequest();"
          "xmlhttp.onreadystatechange = function() {"
              "if (xmlhttp.readyState == XMLHttpRequest.DONE) {"
                "if (xmlhttp.status == 200) {"
                    /* "console.log(\"RES SERVER: \" + xmlhttp.responseText);" */
                    "updateSwitchesUI(xmlhttp.responseText);"
                "}"
                "else {"
                    "console.log('Error', xmlhttp);"
                "}"
              "}"
          "};"
          "xmlhttp.open(\"GET\", \"statusLampara\", true);"
          "xmlhttp.send();"
      "}"
      "(function scheduleAjax() {"
          "ajaxCall();"
          "setTimeout(scheduleAjax, 1000);"
      "})();"

      "function checkTunel() {"
        "var checkbox = document.getElementById('switch1');"
        "if (checkbox.checked == true) {"
          /* "alert(\"ENCIENDE TUNEL\");" */
          "var url = \"/enciendeTunel?enc=1\";"
          "window.location.href = url;"
        "} else {"
          /* "alert(\"APAGA TUNEL\");" */
          "var url = \"/enciendeTunel?enc=0\";"
          "window.location.href = url;"
        "}"
      "}"

      "function checkSuperficies() {"
        "var checkbox = document.getElementById('switch2');"
        "if (checkbox.checked == true) {"
          /* "alert(\"ENCIENDE SUPERFICIE\");" */
          "var url = \"/enciendeSuperficie?enc=1\";"
          "window.location.href = url;"
        "} else {"
          /* "alert(\"APAGA SUPERFICIE\");" */
          "var url = \"/enciendeSuperficie?enc=0\";"
          "window.location.href = url;"
        "}"
      "}"

      "function checkTunelAuto() {"
        "var checkbox = document.getElementById('myonoffswitch1');"
        "if (checkbox.checked == true) {"
          "var url = \"/guardaTunelAuto?auto=1\";"
          "window.location.href = url;"
        "} else {"
          "var url = \"/guardaTunelAuto?auto=0\";"
          "window.location.href = url;"
        "}"
      "}"

      "function checkSupeficiesAuto() {"
        "var checkbox = document.getElementById('myonoffswitch2');"
        "if (checkbox.checked == true) {"
          "var url = \"/guardaSuperficieAuto?auto=1\";"
          "window.location.href = url;"
        "} else {"
          "var url = \"/guardaSuperficieAuto?auto=0\";"
          "window.location.href = url;"
        "}"
      "}"

    "</script>"

  "<title>Protección Pura</title>"
  "</head>"
  "<body>"
    "<div class=\"container\">"
    "<div class=\"nodo-master-container\">"
    "<div class=\"register\" style=\"width:380px\">");
        //page += String(F("<form action=\"/monitorSave\" method=\"post\" name=\"formLampara\">"));
        //page += String(F("<form action=\"javascript:guardarConfig()\" method=\"post\">"));
            page += String(F("<h2>PROTECCIÓN PURA</h2>"));
            page += String(F("<hr>"));
            page += String(F("<h1>")) + nomLampara + String(F("</h1>"));
            page += String(F("<hr>"));
            page += String(F("<h3 class=\"sub_h3\">Encendido Manual</h3>"));

            page += String(F("<table>"));
              page += String(F("<tr>"));
                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<div class=\"mid\">"));
                    page += String(F("<label class=\"rocker\">"));
                      if(statusTunelManual)
                        page += String(F("<input type=\"checkbox\" onclick=\"checkTunel()\" name=\"switch1\" id=\"switch1\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" onclick=\"checkTunel()\" name=\"switch1\" id=\"switch1\">"));
                      page += String(F("<span class=\"switch-left\">On</span>"));
                      page += String(F("<span class=\"switch-right\">Off</span>"));
                    page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));
                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<div class=\"mid\">"));
                    page += String(F("<label class=\"rocker\">"));
                      if(statusSuperficiesManual)
                        page += String(F("<input type=\"checkbox\" onclick=\"checkSuperficies()\" name=\"switch2\" id=\"switch2\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" onclick=\"checkSuperficies()\" name=\"switch2\" id=\"switch2\">"));
                      page += String(F("<span class=\"switch-left\">On</span>"));
                      page += String(F("<span class=\"switch-right\">Off</span>"));
                    page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr>"));
                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<label class=\"formulario_label\">Túnel de Aire</label>"));
                page += String(F("</td>"));

                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<label class=\"formulario_label\">Superficies</label>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>"));

            page += String(F("<hr>"));
            page += String(F("<h3 class=\"sub_h3\">Encendido Automático</h3>"));

            page += String(F("<table>"));
              page += String(F("<tr>"));
                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<div class=\"onoffswitch\">"));
                      if(statusTunelAuto)
                        page += String(F("<input type=\"checkbox\" onclick=\"checkTunelAuto()\" name=\"onoffswitch1\" class=\"onoffswitch-checkbox\" id=\"myonoffswitch1\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" onclick=\"checkTunelAuto()\" name=\"onoffswitch1\" class=\"onoffswitch-checkbox\" id=\"myonoffswitch1\">"));
                      page += String(F("<label class=\"onoffswitch-label\" for=\"myonoffswitch1\">"));
                          page += String(F("<span class=\"onoffswitch-inner\"></span>"));
                          page += String(F("<span class=\"onoffswitch-switch\"></span>"));
                      page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));

                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<div class=\"onoffswitch\">"));
                      if(statusSuperficiesAuto)
                        page += String(F("<input type=\"checkbox\" onclick=\"checkSupeficiesAuto()\" name=\"onoffswitch2\" class=\"onoffswitch-checkbox\" id=\"myonoffswitch2\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" onclick=\"checkSupeficiesAuto()\" name=\"onoffswitch2\" class=\"onoffswitch-checkbox\" id=\"myonoffswitch2\">"));
                      page += String(F("<label class=\"onoffswitch-label\" for=\"myonoffswitch2\">"));
                          page += String(F("<span class=\"onoffswitch-inner\"></span>"));
                          page += String(F("<span class=\"onoffswitch-switch\"></span>"));
                      page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr>"));
                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<label class=\"formulario_label\">Túnel de Aire</label>"));
                page += String(F("</td>"));

                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<label class=\"formulario_label\">Superficies</label>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>"));

            page += String(F("<hr>"));
            page += String(F("<h3 class=\"sub_h3\">Configuración</h3>"));

             page += String(F("<table>"));
              page += String(F("<tr>"));
                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<a href=\"/configHorarios\"><button class=\"button_config\">Horarios Automáticos</button></a>"));
                page += String(F("</td>"));
                page += String(F("<td style=\"width:190px\">"));
                  page += String(F("<a href=\"/configBitacora\"><button class=\"button_config\">Envío Bitácora</button></a>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>"));

            //page += String(F("<input type=\"hidden\" name=\"mac\" value=\"")) + t_mac + String(F("\">"));

            

            //page += String(F("<input type=\"submit\" class=\"submit\" value=\"GUARDAR\">"));

        //page += String(F("</form>"));
        page += String(F("<a href=\"/logout\"><button class=\"botonLogout\">CERRAR SESIÓN</button></a>"));

   page += String(F("</div></div></div>"));
   page += String(F("</body>"));
   page += String(F("</html>"));

  return page;
}


String armaPaginaConfig(String mntCnx){
    String page = PSTR(
  "<html>"
  "<head>"
    "<meta charset=\"UTF-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">"
    //"<link href=\"https://fonts.googleapis.com/css?family=Roboto:100,100i,300,300i,400,400i,500,500i,700,700i,900,900i\" rel=\"stylesheet\">"
    //"<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">"
    "<style type=\"text/css\">"
    "* *{"
      "margin: 0;"
      "padding: 0;"
      "box-sizing: border-box;"
      "font-family: 'Roboto', sans-serif;"
    "}"
    
    "body{"
      "background: #f1f1f1;"
    "}" 
    ".menu > a:link {"
    "position: absolute;"
    "display: inline-block;"
    "right: 12px;"
    "padding: 0 6px;"
    "text-decoration: none;"
    "}"
    ".container{"
      "width: 100%;"
      "height: auto;"
      "padding: 5px;"
      "display: flex;"
      "justify-content: center;"
    "}"
    ".nodo-master-container{"
      "display: flex;"
      "width: auto;"
      "height: auto;"
      "background:#fff;"
      "box-shadow: 0 1px 1px 0 rgba(0, 0, 0, .2);"
    "}"
    ".register{"
      "padding: 10px 10px 0px 10px;"
      "position: relative;"
    "}"
    ".register h2, .register h3, .register h4 {"
      "color: #6a5acd;"
      "margin-top: 15px;"
      "margin-bottom: 10px;"
      "text-align: center;"
    "}"
    ".register input{"
      "width: 96%;"
      "padding: 5px;"
      "font-size: 16px;"
      "margin-left: 2%;"
      "margin-bottom: 15px;"
      "background: rgba(0, 0, 0, 0);"
      "border: 1px solid #a8a8a8;"
      "color: #333;"
    "}"
    ".registerinput:invalid {"
      "border: 1px solid #a8a8a8;"
    "}"
    ".registerinput:valid {"
      "border: 1px dashed red;"
    "}"
    ".register select{"
      "width: 96%;"
      "padding: 5px;"
      "font-size: 16px;"
      "margin-left: 2%;"
      "margin-bottom: 15px;"
      "background: rgba(0, 0, 0, 0);"
      "border: 1px solid #a8a8a8;"
      "color: #333;"
    "}"
    ".register input:active,"
    ".register input:focus{"
      "outline: 1px solid #6a5acd;"
    "}"
    ".submit {"
      "display: block;"
      "width: 96%;"
      "height: 6vh;"
      "padding: 10px;"
      "background: #6a5acd!important;"
      "color: #fff!important;"
      "font-size: 14px;"
      "text-align: center;"
      "margin-bottom: 10px;"
      "margin-left: 2%;"
      "box-shadow: 0 0 3px 0 rgba(0, 0, 0, .2);"
      "cursor: pointer;"
      "border: none;"
    "}"

    ".submit:link {color:#fff; text-decoration:none;}"
    ".submit:visited {color:#fff; text-decoration:none;}"
    ".submit:hover {color:#fff; text-decoration:none;}"

    ".register .cancelar {"
      "width: 95%;"
      "height: 6vh;"
      "padding: 10px;"
      "background: #d0533c!important;"
      "color: #fff!important;"
      "font-size: 12px;"
      "margin-left: 2%;"
      "margin-bottom: 10px;"
      "box-shadow: 0 0 3px 0 rgba(0, 0, 0, .2);"
      "cursor: pointer;"
      "border: none;"
    "}"

    ".botonLogout {"
      "width: 40%;"
      "height: 6vh;"
      "padding: 10px;"
      "background: #762900!important;"
      "color: #fff!important;"
      "font-size: 14px;"
      "margin-top: 30px;"
      "margin-bottom: 10px;"
      "box-shadow: 0 0 3px 0 rgba(0, 0, 0, .2);"
      "cursor: pointer;"
      "border: none;"
    "}"
    
    ".formulario_label {"
      "display: block;" 
      "margin-left: 2%;"
      "margin-bottom: 5px;"
      "font-size: 14px;"
      "font-weight: bold;"
      //"color: #6a5acd;"
    "}"

    "</style>"

  "<title>Seleccionar Monitor</title>"
  "</head>"
  "<body>"
    "<div class=\"container\">"
    "<div class=\"nodo-master-container\">"
    "<div class=\"register\" style=\"width:380px\">");
        page += String(F("<form action=\"/monitorSave\" method=\"post\" name=\"formLampara\">"));
        //page += String(F("<form action=\"javascript:guardarConfig()\" method=\"post\">"));
            page += String(F("<h2>Seleccionar Monitor</h2>"));

            //page += String(F("<input type=\"hidden\" name=\"mac\" value=\"")) + t_mac + String(F("\">"));

            page += String(F("<label for=\"\" class=\"formulario_label\">A continuación se listan los MONITORES disponibles en la red. Selecciona uno para conectar esta Lampara.</label>"));
            page += String(F("<select class=\"formulario_select\" style=\"margin-top: 15px; margin-bottom: 20px;\" name=\"monitorCnx\" value=\"")); page +=mntCnx; page += String(F("\">"));
                page += String(F("<option value="">== Selecciona Modo de Encendido ==</option>"));
                //page += llenaSelectMonitores(mntCnx);
                /*page += String(F("<option value=\"0\" ")); if (t_tipoProg=="0") {page += String(F("selected=\"selected\""));} page += String(F(">Esterilizado Remoto</option>"));
                page += String(F("<option value=\"2\" ")); if (t_tipoProg==E_AUTOMATICO) {page += String(F("selected=\"selected\""));} page += String(F(">Esterilizado Automático</option>"));
                page += String(F("<option value=\"3\" ")); if (t_tipoProg==E_PROGRAMADO) {page += String(F("selected=\"selected\""));} page += String(F(">Esterilizado Horario Fijo</option>"));
                page += String(F("<option value=\"4\" ")); if (t_tipoProg==E_TUNEL_MANUAL) {page += String(F("selected=\"selected\""));} page += String(F(">Tunel Manual</option>"));
                page += String(F("<option value=\"5\" ")); if (t_tipoProg==E_TUNEL_AUTOMATICO) {page += String(F("selected=\"selected\""));} page += String(F(">Tunel Automático</option>"));>*/
            page += String(F("</select>"));

            page += String(F("<input type=\"submit\" class=\"submit\" value=\"GUARDAR\">"));

        page += String(F("</form>"));
        page += String(F("<a href=\"/logout\"><button class=\"botonLogout\">CERRAR SESIÓN</button></a>"));

   page += String(F("</div></div></div>"));
   page += String(F("</body>"));
   page += String(F("</html>"));

  return page;
}