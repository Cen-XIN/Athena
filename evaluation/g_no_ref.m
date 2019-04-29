clc;
gmapping_maps = imageDatastore('./map/gmapping/*.png');

while hasdata(gmapping_maps)
    img = read(gmapping_maps);
    score = brisque(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
gmapping_maps.reset();

while hasdata(gmapping_maps)
    img = read(gmapping_maps);
    score = niqe(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
gmapping_maps.reset();

while hasdata(gmapping_maps)
    img = read(gmapping_maps);
    score = piqe(img);
    fprintf('%0.4f\n', score);
end