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

$process = Start-Process -FilePath $exe -ArgumentList "$test_dir\arq01.in" -RedirectStandardOutput "output.txt" -PassThru -NoNewWindow
$process.WaitForExit()

if ($LASTEXITCODE -ne 0) {
    Write-Host "processo retornou c�digo de erro "$LASTEXITCODE
}
Compare-Object -ReferenceObject (Get-Content "$test_dir\arq01.out") (Get-Content "output.txt")
