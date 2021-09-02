#version 450 core

uniform vec4 fillColor;
uniform vec4 contourColor;
uniform vec4 pointColor;

uniform int rasterCode;

out vec4 color;

void main()
{
    if (rasterCode == 0)
        color = fillColor;
    else if (rasterCode == 1)
        color = contourColor;
    else color = pointColor;
}