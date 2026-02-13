import random

with open('./scene_1000000_spheres.cfg', 'w') as f:
    f.write("scene : {\n")
    f.write("    pos = [0.0, 0.0, 0.0];\n")
    f.write("    dir = [0.0, 0.0, 1.0];\n")
    f.write("    fov = 60.0;\n\n")

    f.write("    objects = (\n")

    idx = 0
    while (idx < 10000000):
        # petite randomisation
        rx = random.uniform(900, -900)
        ry = random.uniform(900, -900)
        rz = random.uniform(900, -900)
        r = 0.5  # radius constant
        # couleurs pseudo-aléatoires mais déterministes
        col_r = random.randint(0, 255)
        col_g =  random.randint(0, 255)
        col_b =  random.randint(0, 255)
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
