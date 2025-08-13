// JS-обгортка, виклик глобальної JSI функції
export function decodeImage(data: Uint8Array): Uint8Array {
  // @ts-ignore
  return global.decodeImage(data);
}
