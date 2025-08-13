import { NativeModulesProxy } from 'expo-modules-core';

type DecodeOptions = {
  premultiplyAlpha?: boolean;
};

type DecodedImage = {
  data: Uint8Array;
  width: number;
  height: number;
};

const { ImageDecoderModule } = NativeModulesProxy;

export async function decodeImage(
  bytes: Uint8Array,
  options?: DecodeOptions
): Promise<DecodedImage> {
  if (!ImageDecoderModule) {
    throw new Error('ImageDecoderModule is not linked properly.');
  }

  const result = await ImageDecoderModule.decodeByteArray(
    Array.from(bytes),
    options?.premultiplyAlpha ?? true
  );

  return {
    data: new Uint8Array(result.data),
    width: result.width,
    height: result.height
  };
}