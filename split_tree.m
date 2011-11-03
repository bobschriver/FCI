function [ codeTable ] = split_tree( currentCode, prob, count )


if numel(count) == 1
    codeTable = struct('count', count, 'code', currentCode);
else

    % Find cumulative sum of probability
    totalProb = cumsum(prob);
    % Define an error vector, then minimizes to find closest midpoint
    probErr = abs(totalProb-(sum(prob)/2));
    [~, midpoint] = min(probErr);

    % Break the probabilities and counts into two halves
    leftHalfProb = prob(1:midpoint);
    rightHalfProb = prob(midpoint+1:end);

    leftHalfCount = count(1:midpoint);
    rightHalfCount = count(midpoint+1:end);
    
    % Send each half to the next branch
    leftTable = split_tree(strcat(currentCode, '0'), leftHalfProb, leftHalfCount);
    rightTable = split_tree(strcat(currentCode, '1'), rightHalfProb, rightHalfCount);

    % Assemble the code tables constructed along each branch
    codeTable = [leftTable, rightTable];
end

