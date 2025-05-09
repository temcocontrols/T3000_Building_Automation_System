# How to extract the names into a config file for the T3000 software:

1. Create a new file called AppConfig.props next to the solution file (T3000 - VS2019.sln)
2. Create custom texts/strings that use custom base name. We only need to change AppOutputName to rebrand the whole software.
3. Import the AppConfig.props to projects that need rebranding with <Import Project="$(SolutionDir)AppConfig.props" Condition="Exists('$(SolutionDir)AppConfig.props')" />
4. Define the strings to use inside PreprocessorDefinitions (ClCompile for c++ file, ResourceCompile for .rc file, .cs file is step 5)
5. For .csproj and .cs, create a new intermediate file from the .csproj file and define strings inside that file
6. See commits for how to use these new definitions.