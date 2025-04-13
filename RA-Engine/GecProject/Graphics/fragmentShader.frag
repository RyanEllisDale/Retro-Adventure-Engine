uniform sampler2D texture;

uniform vec3 targetColours[4];
uniform vec3 replacementColours[4];

void main()
{
    // Lookup the pixel in the texture
    // Current Pixel
    vec4 currentPixel = texture2D(texture, gl_TexCoord[0].xy);

    // Discard fully transparent pixels early:
    // textures masked from alpha mask colour in graphics
    if (currentPixel.a == 0.0)
    {
        discard;
    }

    // Default to original color:
    vec4 outputColor = gl_Color * currentPixel;

    // Replacing colours
    // loop 4 times for each colour, break when replaced a colour
    for (int i = 0; i < 4; i++)
    {
        if (currentPixel.rgb == targetColours[i])
        {
            outputColor = vec4(replacementColours[i], 1.0);
            break; 
        }
    }
    
    gl_FragColor = outputColor;
}
