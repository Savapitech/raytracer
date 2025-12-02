import random

with open('./scene_100000_spheres.cfg', 'w') as f:
    f.write("scene : {\n")
    f.write("    pos = [0.0, 0.0, 0.0];\n")
    f.write("    dir = [0.0, 0.0, 1.0];\n")
    f.write("    fov = 60.0;\n\n")

    f.write("    objects = (\n")

    idx = 0
    while (idx < 100000):
        # petite randomisation
        rx = random.uniform(1000, -1000)
        ry = random.uniform(1000, -1000)
        rz = random.uniform(1000, -1000)
        r = 0.5  # radius constant
        # couleurs pseudo-aléatoires mais déterministes
        col_r = idx % 255
        col_g = idx % 255
        col_b = idx % 255
        f.write("        {\n")
        f.write("            shape = {\n")
        f.write('                type = "sphere";\n')
        f.write(f"                pos = [{rx:.2f}, {ry:.2f}, {rz:.2f}];\n")
        f.write(f"                radius = {r};\n")
        f.write(f"                color = [{col_r}.0, {col_g}.0, {col_b}.0];\n")
        f.write("            };\n")
        f.write("        },\n")
        idx += 1

    f.write("    );\n")
    f.write("};\n")

print("Fichier scene_3000_spheres.cfg généré : 3000 sphères OK")
