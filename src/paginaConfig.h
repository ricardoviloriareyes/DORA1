#include <WString.h>


String armaPaginaConfigHorarios( String nomLampara, int ATTunelHoraInicial, int ATTunelMinutoInicial, int ATTunelHoraFinal, int ATTunelMinutoFinal, int ATSupHoraInicial, int ATSupMinutoInicial, int ATSupHoraFinal, int ATSupMinutoFinal, int ATDurUvc ){
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
      "-webkit-appearance: none;"
      "-webkit-border-radius: 0;"
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
    ".register h2, .register h4 {"
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
      /* "text-align: center !important;" */
    "}"
    ".formulario_table1 {"
      "width: 100%;"
      /* "margin-bottom: 20px;" */
    "}"
    ".formulario_table1 .td1{"
      "width: 70%;"
    "}"
    ".formulario_table1 .td2{"
      "width: 30%;"
      "text-align:right;"
    "}"
    ".formulario_table1 .td1a{"
      "width: 80%;"
    "}"
    ".formulario_table1 .td2a{"
      "width: 20%;"
      "text-align:right;"
    "}"
    ".formulario_label {"
      /* "display: block;" 
      "margin-left: 2%;"
      "margin-bottom: 5px;" */
      "font-size: 16px;"
      "font-weight: bold;"
      "color: #818181;"
    "}"
    ".formulario_input {"
      "width: 50px;"
      "height: 30px !important;"
      "font-size: 16px;"
      "background: rgba(0, 0, 0, 0);"
      "color: #9c9c9c;"
      "border: 1px solid #a8a8a8;"
      "padding: 0 5px 0 5px;"
    "}"
    ".formulario_input2 {"
      "width: 100%;"
      "height: 30px !important;"
      "margin-bottom: 30px;"
      "font-size: 16px;"
      "background: rgba(0, 0, 0, 0);"
      "color: #9c9c9c;"
      "border: 1px solid #a8a8a8;"
      "padding: 0 5px 0 5px;"
    "}"
    ".formulario_input .formulario_input2:invalid {"
      "border: 1px solid red;"
    "}"
    ".formulario_input .formulario_input2:valid {"
      "border: 1px solid #a8a8a8;"
    "}"
    ".formulario_input .formulario_input2:focus {"
      "color: black;"
      "border: 1px solid #a8a8a8;"
    "}"
    

    ".submit {"
      "display: block;"
      "width: 100%;"
      "height: 6vh;"
      "background: #6a5acd!important;"
      "color: #fff!important;"
      "font-size: 14px;"
      "text-align: center;"
      "margin-bottom: 10px;"
      "box-shadow: 0 0 3px 0 rgba(0, 0, 0, .2);"
      "cursor: pointer;"
      "border: none;"
    "}"

    ".submit:link {color:#fff; text-decoration:none;}"
    ".submit:visited {color:#fff; text-decoration:none;}"
    ".submit:hover {color:#fff; text-decoration:none;}"

    ".cancelar {"
      "width: 100%;"
      "height: 6vh;"
      "background: #d0533c !important;"
      "color: #fff!important;"
      "font-size: 14px;"
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
    
    "</style>"

    "<script src=\"https://code.jquery.com/jquery-3.5.1.js\" integrity=\"sha256-QWo7LDvxbWT2tbbQ97B53yJnYU3WhH/C8ycbRAkjPDc=\" crossorigin=\"anonymous\" type=\"text/javascript\"></script>"
    "<script>"
    "const validateHora = idElement => {"
        "const element=document.getElementById(idElement);"

        "const patronHora=new RegExp(/^([01]?[0-9]|2[0-3])$/gm);"
        "if (element.value.search(patronHora)==0) {"
            "element.style.background=\"rgba(0, 0, 0, 0)\";"
        "} else {"
            "element.style.background=\"rgba(208, 83, 60, 0.3)\";"
            /* "element.style.color=\"rgba(0, 0, 0, 0)\";" */
        "}"
    "};"

    "const validateMinutoSegundo = idElement => {"
        "const element=document.getElementById(idElement);"

        "const patronMinutoSegundo=new RegExp(/^([01]?[0-9]|[0-5][0-9])$/gm);"
        "if (element.value.search(patronMinutoSegundo)==0) {"
            "element.style.background=\"rgba(0, 0, 0, 0)\";"
        "} else {"
            "element.style.background=\"rgba(208, 83, 60, 0.3)\";"
            /* "element.style.color=\"rgba(0, 0, 0, 0)\";" */
        "}"
    "};"

    "window.onload = () => {"
      "document.getElementById(\"ATTunelHoraInicial\").addEventListener(\"keyup\", e => {"
        "validateHora(\"ATTunelHoraInicial\");"
      "});"

      "document.getElementById(\"ATTunelMinutoInicial\").addEventListener(\"keyup\", e => {"
        "validateMinutoSegundo(\"ATTunelMinutoInicial\")"
      "});"

      "document.getElementById(\"ATTunelHoraFinal\").addEventListener(\"keyup\", e => {"
        "validateHora(\"ATTunelHoraFinal\")"
      "});"

      "document.getElementById(\"ATTunelMinutoFinal\").addEventListener(\"keyup\", e => {"
        "validateMinutoSegundo(\"ATTunelMinutoFinal\")"
      "});"
      

      "document.getElementById(\"ATSupHoraInicial\").addEventListener(\"keyup\", e => {"
        "validateHora(\"ATSupHoraInicial\");"
      "});"

      "document.getElementById(\"ATSupMinutoInicial\").addEventListener(\"keyup\", e => {"
        "validateMinutoSegundo(\"ATSupMinutoInicial\")"
      "});"

      "document.getElementById(\"ATSupHoraFinal\").addEventListener(\"keyup\", e => {"
        "validateHora(\"ATSupHoraFinal\")"
      "});"

      "document.getElementById(\"ATSupMinutoFinal\").addEventListener(\"keyup\", e => {"
        "validateMinutoSegundo(\"ATSupMinutoFinal\")"
      "});"
    "};"

    "</script>"

  "<title>Protección Pura</title>"
  "</head>"
  "<body>"
    "<div class=\"container\">"
    "<div class=\"nodo-master-container\">"
    "<div class=\"register\" style=\"width:380px\">");
        page += String(F("<form action=\"/guardarHorarios\" method=\"post\" name=\"formLampara\">"));
        //page += String(F("<form action=\"javascript:guardarConfig()\" method=\"post\">"));
            page += String(F("<h2>Horarios de Encendido</h2>"));
            page += String(F("<hr>"));
           /*  page += String(F("<h1>")) + nomLampara + String(F("</h1>"));
            page += String(F("<hr>")); */
            page += String(F("<h3 class=\"sub_h3\">Túnel Automático</h3>"));

            page += String(F("<table class=\"formulario_table1\">"));
              page += String(F("<tr>"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Inicia detección (HR:MIN)</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<table>"));
                    page += String(F("<tr>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATTunelHoraInicial\" name=\"ATTunelHoraInicial\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATTunelHoraInicial; page += String(F("\" required>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<label for=\"\" class=\"formulario_label\"> : </label>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATTunelMinutoInicial\" name=\"ATTunelMinutoInicial\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATTunelMinutoInicial; page += String(F("\" required>"));
                        page += String(F("</td>"));
                     page += String(F("</tr>"));
                  page += String(F("</table>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr>"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Termina detección (HR:MIN)</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<table>"));
                    page += String(F("<tr>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATTunelHoraFinal\" name=\"ATTunelHoraFinal\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATTunelHoraFinal; page += String(F("\" required>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<label for=\"\" class=\"formulario_label\"> : </label>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATTunelMinutoFinal\" name=\"ATTunelMinutoFinal\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATTunelMinutoFinal; page += String(F("\" required>"));
                        page += String(F("</td>"));
                     page += String(F("</tr>"));
                  page += String(F("</table>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>"));

            page += String(F("<hr style=\"margin-top: 20px\">"));
            page += String(F("<h3 class=\"sub_h3\">Superficies Automático</h3>"));

            page += String(F("<table class=\"formulario_table1\">"));
              page += String(F("<tr>"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Inicia detección (HR:MIN)</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<table>"));
                    page += String(F("<tr>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATSupHoraInicial\" name=\"ATSupHoraInicial\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATSupHoraInicial; page += String(F("\" required>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<label for=\"\" class=\"formulario_label\"> : </label>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATSupMinutoInicial\" name=\"ATSupMinutoInicial\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATSupMinutoInicial; page += String(F("\" required>"));
                        page += String(F("</td>"));
                     page += String(F("</tr>"));
                  page += String(F("</table>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr>"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Termina detección (HR:MIN)</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<table>"));
                    page += String(F("<tr>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATSupHoraFinal\" name=\"ATSupHoraFinal\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATSupHoraFinal; page += String(F("\" required>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<label for=\"\" class=\"formulario_label\"> : </label>"));
                        page += String(F("</td>"));
                        page += String(F("<td>"));
                            page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATSupMinutoFinal\" name=\"ATSupMinutoFinal\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATSupMinutoFinal; page += String(F("\" required>"));
                        page += String(F("</td>"));
                     page += String(F("</tr>"));
                  page += String(F("</table>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>"));

            page += String(F("<table class=\"formulario_table1\">"));
              page += String(F("<tr>"));
                page += String(F("<td class=\"td1a\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Duración por desinfección (MIN)</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2a\">"));
                  page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATDurUvc\" name=\"ATDurUvc\" maxlength=\"2\" size=\"2\" value=\"")); page +=ATDurUvc; page += String(F("\" required>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>"));

            page += String(F("<hr style=\"margin-top: 20px\">"));
            page += String(F("<h3 class=\"sub_h3\">Nombre de Unidad</h3>"));
            page += String(F("<input type=\"text\" class=\"formulario_input2\" id=\"nomLampara\" name=\"nomLampara\" maxlength=\"20\" value=\"")); page +=nomLampara; page += String(F("\" required>"));

            page += String(F("<input type=\"submit\" class=\"submit\" value=\"GUARDAR Y SALIR\">"));
            
        page += String(F("</form>"));
        page += String(F("<a href=\"/\"><button class = \"cancelar\">CANCELAR</button></a>"));

        page += String(F("<a href=\"/logout\"><button class=\"botonLogout\">CERRAR SESIÓN</button></a>"));

   page += String(F("</div></div></div>"));
   page += String(F("</body>"));
   page += String(F("</html>"));

  return page;
}


String armaPaginaConfigBitacora( int ATEnviaCorreoTunel, int ATEnviaCorreoSup, int ATEnviaPinDiario, int ATEnviaAlarmaCO2, String ATemail, String ATpassword, String AThost_name, int ATport, String ATreceptoremail ){
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
      "-webkit-appearance: none;"
      "-webkit-border-radius: 0;"
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
    ".register h2, .register h4 {"
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
      /* "text-align: center !important;" */
    "}"
    ".formulario_table1 {"
      "width: 100%;"
      "margin-bottom: 20px;"
    "}"
    ".formulario_table1 .tr1{"
      "height: 42px;"
    "}"
    ".formulario_table1 .td1{"
      "width: 70%;"
    "}"
    ".formulario_table1 .td2{"
      "width: 30%;"
      /* "text-align:right;" */
    "}"
    ".formulario_table1 .td1a{"
      "width: 35%;"
    "}"
    ".formulario_table1 .td2a{"
      "width: 65%;"
      /* "text-align:right;" */
    "}"
    ".formulario_input {"
      "width: 100%;"
      "height: 30px !important;"
      "font-size: 16px;"
      "background: rgba(0, 0, 0, 0);"
      "color: #9c9c9c;"
      "border: 1px solid #a8a8a8;"
      "padding: 0 5px 0 5px;"
    "}"
    ".formulario_input:invalid {"
      "border: 1px solid red;"
    "}"
    ".formulario_input:valid {"
      "border: 1px solid #a8a8a8;"
    "}"
    ".formulario_input:focus {"
      "color: black;"
      "border: 1px solid #a8a8a8;"
    "}"

    ".onoffswitch {"
        "position: relative; width: 60px;"
        "margin-left: auto;"
        "-webkit-user-select:none; -moz-user-select:none; -ms-user-select: none;"
    "}"
    ".onoffswitch-checkbox {"
        "position: absolute;"
        "opacity: 0;"
        "pointer-events: none;"
    "}"
    ".onoffswitch-label {"
        "display: block; overflow: hidden; cursor: pointer;"
        "border: 2px solid ; border-radius: 10px;"
    "}"
    ".onoffswitch-inner {"
        "display: block; width: 200%; margin-left: -100%;"
        "transition: margin 0.3s ease-in 0s;"
    "}"
    ".onoffswitch-inner:before, .onoffswitch-inner:after {"
        "display: block; float: left; width: 50%; height: 32px; padding: 0; line-height: 32px;"
        "font-size: 14px; color: white; font-family: Trebuchet, Arial, sans-serif; font-weight: bold;"
       " box-sizing: border-box;"
    "}"
    ".onoffswitch-inner:before {"
        "content: \"SI\";"
        "padding-left: 6px;"
        "background-color: #1687A7; color: #FFFFFF;"
    "}"
    ".onoffswitch-inner:after {"
        "content: \"NO\";"
        "padding-right: 4px;"
        "background-color: #EEEEEE; color: #999999;"
        "text-align: right;"
    "}"
    ".onoffswitch-switch {"
        "display: block; width: 20px; margin: 6px;"
        "background: #FFFFFF;"
        "position: absolute; top: 0; bottom: 0;"
        "right: 24px;"
        "border: 2px solid ; border-radius: 10px;"
        "transition: all 0.3s ease-in 0s;" 
    "}"
    ".onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-inner {"
        "margin-left: 0;"
    "}"
    ".onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-switch {"
        "right: 0px;" 
    "}"
    

    ".submit {"
      "display: block;"
      "width: 100%;"
      "height: 6vh;"
      "background: #6a5acd!important;"
      "color: #fff!important;"
      "font-size: 14px;"
      "text-align: center;"
      "margin-bottom: 10px;"
      "box-shadow: 0 0 3px 0 rgba(0, 0, 0, .2);"
      "cursor: pointer;"
      "border: none;"
    "}"

    ".submit:link {color:#fff; text-decoration:none;}"
    ".submit:visited {color:#fff; text-decoration:none;}"
    ".submit:hover {color:#fff; text-decoration:none;}"

    ".cancelar {"
      "width: 100%;"
      "height: 6vh;"
      "background: #d0533c !important;"
      "color: #fff!important;"
      "font-size: 14px;"
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
    
    "</style>"

  "<title>Protección Pura</title>"
  "</head>"
  "<body>"
    "<div class=\"container\">"
    "<div class=\"nodo-master-container\">"
    "<div class=\"register\" style=\"width:380px\">");
        page += String(F("<form action=\"/guardarBitacora\" method=\"post\" name=\"formLampara\">"));
            page += String(F("<h2>Configurar Envíos de Bitácoras</h2>"));
            page += String(F("<hr>"));
            page += String(F("<h3 class=\"sub_h3\">Activar/Desactivar envíos por correo</h3>"));

            page += String(F("<table class=\"formulario_table1\">"));
              page += String(F("<tr class=\"tr1\">"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Bitácora de aire</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<div class=\"onoffswitch\">"));
                      if(ATEnviaCorreoTunel)
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaCorreoTunel\" class=\"onoffswitch-checkbox\" id=\"ATEnviaCorreoTunel\" value=\"1\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaCorreoTunel\" class=\"onoffswitch-checkbox\" id=\"ATEnviaCorreoTunel\" value=\"1\">"));
                      page += String(F("<label class=\"onoffswitch-label\" for=\"ATEnviaCorreoTunel\">"));
                        page += String(F("<span class=\"onoffswitch-inner\"></span>"));
                        page += String(F("<span class=\"onoffswitch-switch\"></span>"));
                      page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr class=\"tr1\">"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Bitácora de superficies</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<div class=\"onoffswitch\">"));
                      if(ATEnviaCorreoSup)
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaCorreoSup\" class=\"onoffswitch-checkbox\" id=\"ATEnviaCorreoSup\" value=\"1\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaCorreoSup\" class=\"onoffswitch-checkbox\" id=\"ATEnviaCorreoSup\" value=\"1\">"));
                      page += String(F("<label class=\"onoffswitch-label\" for=\"ATEnviaCorreoSup\">"));
                        page += String(F("<span class=\"onoffswitch-inner\"></span>"));
                        page += String(F("<span class=\"onoffswitch-switch\"></span>"));
                      page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr class=\"tr1\">"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Pin diario</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<div class=\"onoffswitch\">"));
                      if(ATEnviaPinDiario)
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaPinDiario\" class=\"onoffswitch-checkbox\" id=\"ATEnviaPinDiario\" value=\"1\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaPinDiario\" class=\"onoffswitch-checkbox\" id=\"ATEnviaPinDiario\" value=\"1\">"));
                      page += String(F("<label class=\"onoffswitch-label\" for=\"ATEnviaPinDiario\">"));
                        page += String(F("<span class=\"onoffswitch-inner\"></span>"));
                        page += String(F("<span class=\"onoffswitch-switch\"></span>"));
                      page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr class=\"tr1\">"));
                page += String(F("<td class=\"td1\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Correo por Alarma CO2</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2\">"));
                  page += String(F("<div class=\"onoffswitch\">"));
                      if(ATEnviaAlarmaCO2)
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaAlarmaCO2\" class=\"onoffswitch-checkbox\" id=\"ATEnviaAlarmaCO2\" value=\"1\" checked>"));
                      else
                        page += String(F("<input type=\"checkbox\" name=\"ATEnviaAlarmaCO2\" class=\"onoffswitch-checkbox\" id=\"ATEnviaAlarmaCO2\" value=\"1\">"));
                      page += String(F("<label class=\"onoffswitch-label\" for=\"ATEnviaAlarmaCO2\">"));
                        page += String(F("<span class=\"onoffswitch-inner\"></span>"));
                        page += String(F("<span class=\"onoffswitch-switch\"></span>"));
                      page += String(F("</label>"));
                  page += String(F("</div>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>"));

            page += String(F("<hr>"));
            page += String(F("<h3 class=\"sub_h3\">Configuración SMTP</h3>"));

            page += String(F("<table class=\"formulario_table1\">"));
              page += String(F("<tr>"));
                page += String(F("<td class=\"td1a\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Usuario:</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2a\">"));
                  page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATemail\" name=\"ATemail\" maxlength=\"30\" size=\"30\" value=\"")); page +=ATemail; page += String(F("\" required>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr>"));
                page += String(F("<td class=\"td1a\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Contraseña:</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2a\">"));
                  page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATpassword\" name=\"ATpassword\" maxlength=\"30\" size=\"30\" value=\"")); page +=ATpassword; page += String(F("\" required>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr>"));
                page += String(F("<td class=\"td1a\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Servidor SMTP:</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2a\">"));
                  page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"AThost_name\" name=\"AThost_name\" maxlength=\"30\" size=\"30\" value=\"")); page +=AThost_name; page += String(F("\" required>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
              page += String(F("<tr>"));
                page += String(F("<td class=\"td1a\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Puerto SMTP:</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2a\">"));
                  page += String(F("<input type=\"number\" class=\"formulario_input\" id=\"ATport\" name=\"ATport\" min=\"0\" max=\"9999\" value=\"")); page +=ATport; page += String(F("\" required>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));

              page += String(F("<tr>"));
                page += String(F("<td class=\"td1a\">"));
                  page += String(F("<label for=\"\" class=\"formulario_label\">Email Destino:</label>"));
                page += String(F("</td>"));
                page += String(F("<td class=\"td2a\">"));
                  page += String(F("<input type=\"text\" class=\"formulario_input\" id=\"ATreceptoremail\" name=\"ATreceptoremail\" maxlength=\"30\" size=\"30\" value=\"")); page +=ATreceptoremail; page += String(F("\" required>"));
                page += String(F("</td>"));
              page += String(F("</tr>"));
            page += String(F("</table>")); 

            page += String(F("<input type=\"submit\" class=\"submit\" value=\"GUARDAR Y SALIR\">"));
            
        page += String(F("</form>"));
        /// correccion ricardo, le quita el lampara al 
        page += String(F("<a href=\"/\"><button class = \"cancelar\">CANCELAR</button></a>"));

        page += String(F("<a href=\"/logout\"><button class=\"botonLogout\">CERRAR SESIÓN</button></a>"));

    page += String(F("</div></div></div>"));
  page += String(F("</body>"));
  page += String(F("</html>"));

  return page;
}