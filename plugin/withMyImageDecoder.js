const { createRunOncePlugin } = require('@expo/config-plugins');

const withMyImageDecoder = (config) => {
  // Якщо потрібно, додати кастомні налаштування у android/app/build.gradle або gradle.properties
  return config;
};

module.exports = createRunOncePlugin(withMyImageDecoder, 'with-my-image-decoder', '1.0.0');
