# ============================================================================
#  The Betrayed Will / وصية الغدر — what is actually in a model folder?
#
#  Before importing anything, answer the one question that decides the whole
#  pipeline: IS IT RIGGED?
#
#      a mesh with a skeleton  -> can be a character, can play our animations
#      a mesh without one      -> a statue. Beautiful, and it cannot walk.
#
#  Tools like Tripo3D usually output an unrigged mesh. That is not a fault, it
#  is what photogrammetry-style generators do. But importing one and hoping is
#  how an evening disappears.
#
#  This reads the files without Unreal and without Blender, and says what it
#  found: skeletons, skin weights, animation tracks, textures, triangle-ish
#  size, and the licence file if the exporter left one.
#
#  Usage:
#    .\tools\INSPECT_MODEL.cmd "C:\Users\dell\Downloads\Tripo3d_Blender_Bridge"
# ============================================================================

param(
    [Parameter(Position = 0)]
    [string]$Path = "",
    [switch]$Find
)

$ErrorActionPreference = "Continue"

if ($Find) {
    # Where an export actually lands: the browser's download folder, the
    # desktop, Documents, and whatever OneDrive has redirected those to.
    Write-Host ""
    Write-Host "============================================================" -ForegroundColor Cyan
    Write-Host "   LOOKING FOR 3D FILES" -ForegroundColor Cyan
    Write-Host "============================================================" -ForegroundColor Cyan
    $roots = @(
        [Environment]::GetFolderPath("UserProfile") + "\Downloads",
        [Environment]::GetFolderPath("Desktop"),
        [Environment]::GetFolderPath("MyDocuments"),
        [Environment]::GetFolderPath("UserProfile") + "\OneDrive\Desktop",
        [Environment]::GetFolderPath("UserProfile") + "\OneDrive\Documents"
    ) | Where-Object { Test-Path $_ } | Select-Object -Unique

    $wanted = @("*.fbx", "*.glb", "*.gltf", "*.obj", "*.usd", "*.usdz", "*.blend", "*.zip")
    $hits = @()
    foreach ($r in $roots) {
        Write-Host "   scanning $r ..."
        foreach ($w in $wanted) {
            $hits += @(Get-ChildItem -Path $r -Filter $w -Recurse -File -ErrorAction SilentlyContinue)
        }
    }
    Write-Host ""
    if ($hits.Count -eq 0) {
        Write-Host " Nothing found. The model has not been exported yet." -ForegroundColor Yellow
        Write-Host " In Blender: File > Export > FBX (or glTF 2.0), tick Apply Transform."
    } else {
        Write-Host " NEWEST FIRST" -ForegroundColor Green
        $hits | Sort-Object LastWriteTime -Descending | Select-Object -First 20 |
            ForEach-Object {
                Write-Host ("   {0:yyyy-MM-dd HH:mm}  {1,8:N1} MB  {2}" -f
                            $_.LastWriteTime, ($_.Length / 1MB), $_.FullName)
            }
        Write-Host ""
        Write-Host " Then inspect the folder one of these is in:"
        Write-Host '   .\tools\INSPECT_MODEL.cmd "<that folder>"'
    }
    Write-Host ""
    exit 0
}

if (-not $Path) {
    Write-Host ""
    Write-Host " Give me the folder:" -ForegroundColor Yellow
    Write-Host '   .\tools\INSPECT_MODEL.cmd "C:\Users\dell\Downloads\Tripo3d_Blender_Bridge"'
    Write-Host ""
    exit 1
}
if (-not (Test-Path $Path)) {
    Write-Host ""
    Write-Host " Not found: $Path" -ForegroundColor Red
    Write-Host ""
    exit 1
}

Clear-Host
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   MODEL INSPECTION" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host " $Path"
Write-Host ""

$report = New-Object System.Collections.Generic.List[string]
function Say($t, $c = "Gray") { Write-Host $t -ForegroundColor $c; $report.Add($t) }

$all = @(Get-ChildItem -Path $Path -Recurse -File -ErrorAction SilentlyContinue)
if ($all.Count -eq 0) { Say " The folder is empty." "Red"; exit 1 }

Say (" files: {0}   total: {1} MB" -f $all.Count,
     [math]::Round(($all | Measure-Object Length -Sum).Sum / 1MB, 1))
Say ""

# ------------------------------------------------------------- by extension
Say " BY TYPE"
$all | Group-Object Extension | Sort-Object Count -Descending | ForEach-Object {
    Say ("   {0,-8} {1,3} file(s)   {2,8:N1} MB" -f
         ($_.Name -replace '^$', '(none)'), $_.Count,
         (($_.Group | Measure-Object Length -Sum).Sum / 1MB))
}
Say ""

# ------------------------------------------------------------------ meshes
$meshExt = @(".fbx", ".glb", ".gltf", ".obj", ".usd", ".usdz", ".blend", ".dae", ".ply", ".stl")
$meshes  = @($all | Where-Object { $meshExt -contains $_.Extension.ToLower() })

Say " MESH FILES"
if ($meshes.Count -eq 0) {
    Say "   none found - nothing here can be imported as geometry." "Red"
} else {
    foreach ($m in $meshes) {
        Say ("   {0}   {1:N1} MB" -f $m.Name, ($m.Length / 1MB))
    }
}
Say ""

# ------------------------------------------------------- rigged, or a statue
Say " IS IT RIGGED?"
$anyRigged = $false

foreach ($m in $meshes) {
    $ext = $m.Extension.ToLower()
    $verdict = "unknown"
    $detail = ""

    if ($ext -eq ".obj" -or $ext -eq ".stl" -or $ext -eq ".ply") {
        # These formats cannot carry a skeleton at all. Not an opinion.
        $verdict = "NO SKELETON"
        $detail = "$ext cannot store one - the format has no concept of bones"
    }
    elseif ($ext -eq ".glb" -or $ext -eq ".gltf") {
        try {
            $json = $null
            if ($ext -eq ".gltf") {
                $json = Get-Content $m.FullName -Raw | ConvertFrom-Json
            } else {
                # GLB: 12-byte header, then chunk length + type + JSON payload.
                $bytes = [System.IO.File]::ReadAllBytes($m.FullName)
                $len = [BitConverter]::ToUInt32($bytes, 12)
                $text = [System.Text.Encoding]::UTF8.GetString($bytes, 20, $len)
                $json = $text | ConvertFrom-Json
            }
            $skins = @($json.skins).Count
            $anims = @($json.animations).Count
            $nodes = @($json.nodes).Count
            if ($skins -gt 0) {
                $verdict = "RIGGED"
                $anyRigged = $true
                $detail = "$skins skin(s), $anims animation(s), $nodes node(s)"
            } else {
                $verdict = "NO SKELETON"
                $detail = "0 skins, $nodes node(s) - geometry only"
            }
        } catch {
            $detail = "could not parse: $($_.Exception.Message)"
        }
    }
    elseif ($ext -eq ".fbx") {
        # Works for binary and ASCII FBX alike: the class names appear as
        # literal strings in both encodings.
        try {
            $bytes = [System.IO.File]::ReadAllBytes($m.FullName)
            $text = [System.Text.Encoding]::ASCII.GetString($bytes)
            $hasLimb = $text -match "LimbNode"
            $hasSkin = $text -match "Deformer" -and $text -match "Cluster"
            $hasAnim = $text -match "AnimationCurveNode"
            if ($hasLimb -or $hasSkin) {
                $verdict = "RIGGED"
                $anyRigged = $true
                $bones = ([regex]::Matches($text, "LimbNode")).Count
                $detail = "~$bones bone reference(s)"
                if ($hasSkin) { $detail += ", skin weights present" }
                if ($hasAnim) { $detail += ", animation tracks present" }
            } else {
                $verdict = "NO SKELETON"
                $detail = "no LimbNode and no skin cluster - geometry only"
            }
        } catch {
            $detail = "could not read: $($_.Exception.Message)"
        }
    }
    elseif ($ext -eq ".blend") {
        $verdict = "BLEND FILE"
        $detail = "Unreal cannot import .blend. Export FBX or glTF from Blender first"
    }

    $colour = switch ($verdict) {
        "RIGGED"      { "Green" }
        "NO SKELETON" { "Yellow" }
        default       { "Gray" }
    }
    Write-Host ("   {0,-14} {1}" -f $verdict, $m.Name) -ForegroundColor $colour
    $report.Add(("   {0,-14} {1}" -f $verdict, $m.Name))
    if ($detail) { Say ("                  {0}" -f $detail) }
}
Say ""

# ---------------------------------------------------------------- textures
$texExt = @(".png", ".jpg", ".jpeg", ".tga", ".exr", ".tif", ".tiff", ".webp")
$tex = @($all | Where-Object { $texExt -contains $_.Extension.ToLower() })
Say " TEXTURES"
if ($tex.Count -eq 0) {
    Say "   none - the model will import untextured (flat grey)."
} else {
    foreach ($x in ($tex | Sort-Object Length -Descending | Select-Object -First 12)) {
        Say ("   {0,-44} {1,7:N2} MB" -f $x.Name, ($x.Length / 1MB))
    }
    if ($tex.Count -gt 12) { Say ("   ... and {0} more" -f ($tex.Count - 12)) }
}
Say ""

# ----------------------------------------------------------------- licence
Say " LICENCE / TERMS FILES"
$lic = @($all | Where-Object { $_.Name -match "(?i)licen[cs]e|terms|readme|copyright" })
if ($lic.Count -eq 0) {
    Say "   none in the folder." "Yellow"
    Say "   The exporter's terms decide whether this can ship on Steam." "Yellow"
} else {
    foreach ($l in $lic) { Say ("   {0}" -f $l.FullName) }
}
Say ""

Say "============================================================"
if ($meshes.Count -eq 0) {
    # The first version answered "no skeleton, so it is a statue" for a folder
    # containing no geometry at all. That is a wrong answer to an unasked
    # question, and it points at the wrong next step. No mesh is its own case.
    $py = @($all | Where-Object { $_.Extension -eq ".py" }).Count
    $isAddon = ($py -ge 5) -and (
        ($all | Where-Object { $_.Name -eq "__init__.py" -or $_.Name -eq "blender_manifest.toml" }).Count -gt 0
        -or $py -ge 20)

    Say " VERDICT: there is no 3D model here." "Yellow"
    if ($isAddon) {
        Say ""
        Say " $py Python files and no geometry: this is the Blender ADD-ON," "Yellow"
        Say " the tool that talks to the generator. It is the fishing rod, not"
        Say " the fish. The model it produces is exported somewhere else."
    }
    Say ""
    Say " Find the actual export with:"
    Say '   .\tools\INSPECT_MODEL.cmd -Find'
} elseif ($anyRigged) {
    Say " VERDICT: at least one file carries a skeleton." "Green"
    Say " It can become a character. Next step is retargeting our clips onto it."
} else {
    Say " VERDICT: no skeleton in any of the $($meshes.Count) mesh file(s)." "Yellow"
    Say " As-is it is a STATUE: it can be a prop, a body on a bed, a"
    Say " decoration - but it cannot walk until something rigs it."
}
Say "============================================================"
Write-Host ""

. (Join-Path $PSScriptRoot "ue58_common.ps1")
Copy-TBWReport -Title "Model inspection" -Text ($report -join "`r`n") | Out-Null
exit 0
