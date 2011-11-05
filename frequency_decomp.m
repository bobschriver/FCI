function [ img_LF, img_HF ] = frequency_decomp( img, scale )
% Inputs an image and scale to break into HF and LF components. Larger
% scale values put more data in the HF band.

% Directly adding LF and HF bands after conversion yields the original
% image, +/- .25 digital counts. Rebinning will return original image
% exactly.


%%%%%%%%%%%%%%%%%%%%%%%%%%%  Kernel Generation  %%%%%%%%%%%%%%%%%%%%%%%%%%%

% find size of image
[numRows, numCols] = size(img);

%Define bounds for gaussian kernel
highBoundX = numCols - floor(numCols)/2;
lowBoundX = 1-floor(numCols)/2;
highBoundY = numRows - floor(numRows)/2;
lowBoundY = 1-floor(numCols)/2;

% Define sigma values for X and Y (kept the same for this implementation).
% Scale value set by user to set LF and HF cutoff point
sigX = numRows/scale;
sigY = sigX;
% Define gaussian kernel
kernel_low = repmat(exp((-(lowBoundX:highBoundX).^2)./(2*sigX^2)), numRows, 1);
kernel_low = kernel_low.*repmat(exp((-(lowBoundY:highBoundY).^2)./(2*sigY^2))', 1, numCols);

% Peak-Normalize low frequency
kernel_low = kernel_low./max(max(kernel_low));

% Find the orthogonal filter
kernel_high = 1-kernel_low;

% Clean up both filters to ensure that no values are reduced to zero during
% multiplication
kernel_low = kernel_low*.999;
kernel_low = kernel_low + (1-max(max(kernel_low)));
kernel_high = kernel_high*.999;
kernel_high = kernel_high + (1-max(max(kernel_high)));



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  Computation  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% compute the FFT of the image
img_fft = fftshift(fft2(img));

% Apply kernels to the LF and HF bands
img_fft_LF = img_fft.*kernel_low;
img_fft_HF = img_fft.*kernel_high;

% Return the inverse FFT of both bands to the user.
img_LF = real(ifft2(ifftshift(img_fft_LF)));
img_HF = real(ifft2(ifftshift(img_fft_HF)));

end