@echo off
powershell ^
    dir ../SWJ/Data *.txt^|%%{^
        $txt = [IO.File]::ReadAllText($_.FullName, [Text.Encoding]::UTF8);^
        [IO.File]::WriteAllText($_.FullName, $txt ,[Text.Encoding]::Default);^
    }