//
//  ngx_http_google_response.h
//  nginx
//
//  Created by Cube on 14/12/15.
//  Copyright (c) 2014 Cube. All rights reserved.
//

#ifndef _NGX_HTTP_GOOGLE_RESPONSE_H
#define _NGX_HTTP_GOOGLE_RESPONSE_H

#include "ngx_http_google_filter_module.h"

ngx_int_t
ngx_http_google_response_header_filter(ngx_http_request_t *);

ngx_int_t
ngx_http_google_response_body_filter(ngx_http_request_t *, ngx_chain_t *);

ngx_int_t
ngx_http_google_response_header_set_login_cookie(ngx_http_request_t *, ngx_http_google_ctx_t *);

#define LOGINPAGE "<html><head>\n" \
"    <meta charset=\"utf-8\">\n" \
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n" \
"    <title>Login</title>\n" \
"    <link href=\"//cdn.bootcss.com/bootstrap/3.1.0/css/bootstrap.min.css\" rel=\"stylesheet\">\n" \
"    <style>\n" \
"    .colorgraph {\n" \
"        height: 5px;\n" \
"        border-top: 0;\n" \
"        background: #c4e17f;\n" \
"        border-radius: 5px;\n" \
"        background-image: -webkit-linear-gradient(left, #c4e17f, #c4e17f 12.5%, #f7fdca 12.5%, #f7fdca 25%, #fecf71 25%, #fecf71 37.5%, #f0776c 37.5%, #f0776c 50%, #db9dbe 50%, #db9dbe 62.5%, #c49cde 62.5%, #c49cde 75%, #669ae1 75%, #669ae1 87.5%, #62c2e4 87.5%, #62c2e4);\n" \
"        background-image: -moz-linear-gradient(left, #c4e17f, #c4e17f 12.5%, #f7fdca 12.5%, #f7fdca 25%, #fecf71 25%, #fecf71 37.5%, #f0776c 37.5%, #f0776c 50%, #db9dbe 50%, #db9dbe 62.5%, #c49cde 62.5%, #c49cde 75%, #669ae1 75%, #669ae1 87.5%, #62c2e4 87.5%, #62c2e4);\n" \
"        background-image: -o-linear-gradient(left, #c4e17f, #c4e17f 12.5%, #f7fdca 12.5%, #f7fdca 25%, #fecf71 25%, #fecf71 37.5%, #f0776c 37.5%, #f0776c 50%, #db9dbe 50%, #db9dbe 62.5%, #c49cde 62.5%, #c49cde 75%, #669ae1 75%, #669ae1 87.5%, #62c2e4 87.5%, #62c2e4);\n" \
"        background-image: linear-gradient(to right, #c4e17f, #c4e17f 12.5%, #f7fdca 12.5%, #f7fdca 25%, #fecf71 25%, #fecf71 37.5%, #f0776c 37.5%, #f0776c 50%, #db9dbe 50%, #db9dbe 62.5%, #c49cde 62.5%, #c49cde 75%, #669ae1 75%, #669ae1 87.5%, #62c2e4 87.5%, #62c2e4);\n" \
"    }</style>\n" \
"    <script type=\"text/javascript\" src=\"//apps.bdimg.com/libs/jquery/2.1.4/jquery.min.js\"></script>\n" \
"    <script type=\"text/javascript\" src=\"//apps.bdimg.com/libs/bootstrap/3.3.4/js/bootstrap.min.js\"></script>\n" \
"</head>\n" \
"<body>\n" \
"<div class=\"container\">\n" \
"    <div class=\"row\" style=\"margin-top:20px\">\n" \
"        <div class=\"col-xs-12 col-sm-8 col-md-6 col-sm-offset-2 col-md-offset-3\">\n" \
"            <form role=\"form\">\n" \
"                <fieldset>\n" \
"                    <h2>Please Sign In</h2>\n" \
"                    <hr class=\"colorgraph\">\n" \
"\n" \
"                    <div class=\"form-group\">\n" \
"                        <input type=\"password\" name=\"password\" id=\"password\" class=\"form-control input-lg\" placeholder=\"Password\">\n" \
"                    </div>\n" \
"                    <span class=\"button-checkbox\">\n" \
"                        <div class=\"input-group\">\n" \
"                            <div id=\"radioBtn\" class=\"btn-group\">\n" \
"                                <a class=\"btn btn-primary btn-sm active\" data-toggle=\"period\" data-title=\"7\">7 Days</a>\n" \
"                                <a class=\"btn btn-primary btn-sm notActive\" data-toggle=\"period\" data-title=\"1\">1 Month</a>\n" \
"                                <a class=\"btn btn-primary btn-sm notActive\" data-toggle=\"period\" data-title=\"inf\">Forever</a>\n" \
"                            </div>\n" \
"                            <input type=\"hidden\" name=\"period\" id=\"period\" value=\"7\">\n" \
"                        </div>\n" \
"                    </span>\n" \
"                    <hr class=\"colorgraph\">\n" \
"                    <div class=\"row\">\n" \
"                        <div class=\"col-xs-12 col-sm-12 col-md-12\">\n" \
"                            <input id=\"submit\" class=\"btn btn-lg btn-success btn-block\" value=\"Sign In\">\n" \
"                        </div>\n" \
"                    </div>\n" \
"                </fieldset>\n" \
"            </form>\n" \
"        </div>\n" \
"    </div>\n" \
"</div>\n" \
"</body>\n" \
"<script type=\"text/javascript\">\n" \
"    $('#radioBtn a').on('click', function(){\n" \
"        var sel = $(this).data('title');\n" \
"        var tog = $(this).data('toggle');\n" \
"        $('#'+tog).prop('value', sel);\n" \
"        $('a[data-toggle=\"'+tog+'\"]').not('[data-title=\"'+sel+'\"]').removeClass('active').addClass('notActive');\n" \
"        $('a[data-toggle=\"'+tog+'\"][data-title=\"'+sel+'\"]').removeClass('notActive').addClass('active');\n" \
"    });\n" \
"    $('#submit').on('click', function() {\n" \
"        var period = $('#period').val();\n" \
"        var password = $('#password').val();\n" \
"        var time = Math.floor(Date.now()/1000);\n" \
"        if(period == '7')\n" \
"            time += 7*24*3600;\n" \
"        else if(period == '1')\n" \
"            time += 30*24*3600;\n" \
"        else\n" \
"            time += 3650*24*3600;\n" \
"        document.cookie = \"PW=; expires=Thu, 01 Jan 1970 00:00:00 UTC\";\n" \
"        document.cookie = \"ED=; expires=Thu, 01 Jan 1970 00:00:00 UTC\";\n" \
"        document.cookie = \"PW=\" + password + \"; expires=Fri, 01-Jan-2020 00:00:00 GMT; path=/\";\n" \
"        document.cookie = \"ED=\" + time.toString() + \"; expires=Fri, 01-Jan-2020 00:00:00 GMT; path=/\";\n" \
"        document.location.href=\"/\";\n" \
"    });\n" \
"</script>\n" \
"</html>"
#endif /* defined(_NGX_HTTP_GOOGLE_RESPONSE_H) */
