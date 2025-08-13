const {
  withGradleProperties,
  createRunOncePlugin,
} = require('@expo/config-plugins');

/**
 * Увімкнути нову архітектуру в android/gradle.properties під час EAS/prebuild.
 */
const withMyImageDecoder = (config) => {
  return withGradleProperties(config, (config) => {
    const props = config.modResults;
    const has = props.find((p) => p.type === 'property' && p.key === 'newArchEnabled');
    if (!has) {
      props.push({ type: 'property', key: 'newArchEnabled', value: 'true' });
    } else {
      has.value = 'true';
    }
    return config;
  });
};

module.exports = createRunOncePlugin(
  withMyImageDecoder,
  'with-my-image-decoder',
  '1.0.0'
);
