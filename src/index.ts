/**
 * JS-обгортка над JSI-функцією. Вона інсталюється нативною ініціалізацією.
 * Під капотом ми додаємо глобальну функцію: global.__mid_decodeImage(u8, premul)
 */
type DecodeResult = { data: Uint8Array; width: number; height: number };

export function decodeImage(
  data: Uint8Array,
  options: { premultiplyAlpha: boolean } = { premultiplyAlpha: true }
): DecodeResult {
  // @ts-ignore - глобалку додає JSI під час ініціалізації
  const fn = global.__mid_decodeImage;
  if (typeof fn !== 'function') {
    throw new Error(
      'my-image-decoder: JSI is not installed. Ensure EAS build (not Expo Go) and new architecture enabled.'
    );
  }
  return fn(data, !!options.premultiplyAlpha);
}
