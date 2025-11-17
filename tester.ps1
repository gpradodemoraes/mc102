param (
	# executable
	[Parameter(Mandatory=$true)]
	[string]$exe,
	
	# dir with test files
	[Parameter(Mandatory=$true)]
	[string]$test_dir
)

# .\tester.ps1 -exe Joomba1\out\build\default\Joomba1.exe -test_dir .\Joomba1\testes

Write-Host "$exe $test_dir"

if (-Not (Test-Path -Path $test_dir -PathType Container)) {
	Write-Host "diretório $test_dir não foi encontrado ou não é um diretório"
	exit 1
}

if (-Not (Test-Path -Path $exe -PathType Leaf)) {
	Write-Host "programa $exe não é um arquivo"
	exit 1
}
# if (-Not((Get-Item $exe).Attributes -match "Executable")) {
# 	Write-Host "programa $exe não é executável"
# 	exit 1
# }
foreach ($file in Get-ChildItem -Path $test_dir -Filter "*.in") {
    Write-Host "==> $file"
    $output_name = $file -replace '.in$','.out'
    Write-Host "<== $output_name"
    $TempFile = New-TemporaryFile
    $process = Start-Process -FilePath $exe -ArgumentList $file -RedirectStandardOutput $TempFile -PassThru -NoNewWindow
    $process.WaitForExit()

    if ($LASTEXITCODE -ne 0) {
        Write-Host "processo retornou código de erro "$LASTEXITCODE
		exit 1
    }
    Compare-Object -ReferenceObject (Get-Content $output_name) -DifferenceObject (Get-Content $TempFile)

}


