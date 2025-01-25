module.exports = {
    preset: 'ts-jest', // For TypeScript
    testEnvironment: 'node',
    transform: {
        '^.+\\.tsx?$': 'ts-jest',  // Transform TypeScript files
    },
    extensionsToTreatAsEsm: ['.ts', '.tsx'],  // Treat TypeScript files as ES Modules
    transformIgnorePatterns: [
        '/node_modules/(?!some-es-module|another-es-module)',  // Allow certain modules to be transformed if necessary
    ],
};
