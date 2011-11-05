function [ tempImgArray ] = unwrap_huffman( huffman_codestream, dictionary )

%Convert to binary and unwrap
str = dec2bin(huffman_codestream)';
str = str(:)';

% Generate an array the same size as the image (assuming 512x512 for now)
tempImgArray = zeros(1, 512^2);

% Extract the codes into a cell array for easy comparison
codes = {dictionary.code};


% Initialize counters
i=0; % Current code being read
pos = 1; % Position of first bit of current code

h = waitbar(0, 'Progress');

% Go through every code to be read
while i <= 512^2
    i = i+1;
    % Update the waitbar periodically
    if mod(i, 500) == 0
        waitbar(i/512^2, h);
    end
    
    % Prune possible matches by comparing more and more bits until a single
    % match is found.
    numPossibles = inf;
    n=0;
    while numPossibles > 1
        n=n+1;
        possibles = strncmp(str(pos:end), codes, n);
        numPossibles = sum(possibles);
    end

    % Lookup the count associated with the code. Use zero if the code was
    % not found for some reason, and tell the user!
    if numPossibles == 1
        tempImgArray(i) = dictionary(possibles).count;
    else
        tempImgArray(i) = 0;
        print('Oops');
    end
        

    % Increment position to the necessary point.
    pos = pos+n+1;
end

end

