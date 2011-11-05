function [ Y, cb, cr ] = rgb2ycc( image )
% User Reads in image to be converted from RGB color space to Ycbcr.
% Returns Y, cb, cr seperatley

% Double values of image to increase precision. 
% Eliminating computational error.
r = double(image(:,:,1));
g = double(image(:,:,2));
b = double(image(:,:,3));

% Convert RGB values to Ycbcr, outputting Ycrcb values.
% Color conversion functions match the same functions used in JPEG.
Y = uint8( 0.299*r + 0.587*g + 0.114*b );
cb  = uint8( -0.16874*r  - 0.331126*g + 0.5*b + 128 );
cr = uint8( 0.5*r - 0.41896*g - 0.08131*b + 128 );

end
