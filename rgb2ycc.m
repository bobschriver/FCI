function [ Y, cb, cr ] = rgb2ycc( image )
% User Reads in image to be converted from RGB color space to Ycbcr.
% Returns Y, cb, cr seperatley

% Double values of image to increase precision. (scale zero to 1)
% Eliminating computational error.
r = im2double(image(:,:,1));
g = im2double(image(:,:,2));
b = im2double(image(:,:,3));

% Convert RGB values to Ycbcr, outputting Ycrcb values.
% Color conversion functions match the same functions used in JPEG.
Y = 0.299*r + 0.587*g + 0.114*b;
cb  = -0.16874*r  - 0.331126*g + 0.5*b+.5;
cr = 0.5*r - 0.41896*g - 0.08131*b+.5;

end
