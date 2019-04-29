clc;
hector_maps = imageDatastore('./map/hector/*.png');

while hasdata(hector_maps)
    img = read(hector_maps);
    score = brisque(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
hector_maps.reset();

while hasdata(hector_maps)
    img = read(hector_maps);
    score = niqe(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
hector_maps.reset();

while hasdata(hector_maps)
    img = read(hector_maps);
    score = piqe(img);
    fprintf('%0.4f\n', score);
end
