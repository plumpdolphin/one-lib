import python.minify as minify



def test_css_basic_minification():
    css = 'body { color: red; }'
    minified = minify.css( css )
    assert( minified )
    assert( minified == 'body{color:red}' )

def test_css_comment():
    css = '/*a 9-*/{*/**/}*/{/*a 9-*/}'
    minified = minify.css( css )
    assert( minified == '{*}*/{}')

def test_css_psuedo_class():
    css = 'a:hover { text-decoration: underline; }'
    minified = minify.css( css )
    assert( minified == 'a:hover{text-decoration:underline}' )

def test_css_trailing_semicolon():
    css = '{a:a;}{a:a; }{a:a ; }'
    minified = minify.css( css )
    assert( minified == '{a:a}{a:a}{a:a}')

def test_css_media_query():
    css = '@media screen and (max-width: 600px) { body { font-size: 14px; } }'
    minified = minify.css( css )
    assert( minified == '@media screen and (max-width:600px){body{font-size:14px}}' )