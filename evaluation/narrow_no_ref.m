clc;
narrow_maps = imageDatastore('./map/narrow_space/*.png');

while hasdata(narrow_maps)
    img = read(narrow_maps);
    score = brisque(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
narrow_maps.reset();

while hasdata(narrow_maps)
    img = read(narrow_maps);
    score = niqe(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
narrow_maps.reset();

while hasdata(narrow_maps)
    img = read(narrow_maps);
    score = piqe(img);
    fprintf('%0.4f\n', score);
end
