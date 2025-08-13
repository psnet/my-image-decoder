package com.example.imagedecoder;

import expo.modules.core.BasePackage;
import expo.modules.core.Module;

import java.util.Arrays;
import java.util.List;

public class ImageDecoderPackage extends BasePackage {
    @Override
    public List<Module> createModules() {
        return Arrays.asList(new ImageDecoderModule());
    }
}