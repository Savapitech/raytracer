import random

with open('./scene_3000_spheres.cfg', 'w') as f:
    f.write("scene : {\n")
    f.write("    pos = [0.0, 0.0, -1000.0];\n")
    f.write("    dir = [0.0, 0.0, 1.0];\n")
    f.write("    fov = 60.0;\n\n")

    f.write("    objects = (\n")

    idx = 0
    for x in range(-20, 20, 2):
        for y in range(-10, 10, 2):
            for z in range(0, 120, 4):  # profondeur augmentée
                if idx >= 3000:
                    break

                # petite randomisation
                rx = random.uniform(10, -10)
                ry = random.uniform(10, -10)
                rz = random.uniform(10, -10)

                r = 0.5  # radius constant

                # couleurs pseudo-aléatoires mais déterministes
                col_r = (x * 5) % 255
                col_g = (y * 12) % 255
                col_b = (z * 4) % 255

                f.write("        {\n")
                f.write("            shape = {\n")
                f.write('                type = "sphere";\n')
                f.write(f"                pos = [{rx:.2f}, {ry:.2f}, {rz:.2f}];\n")
                f.write(f"                radius = {r};\n")
                f.write(f"                color = [{col_r}.0, {col_g}.0, {col_b}.0];\n")
                f.write("            };\n")
                f.write("        },\n")

                idx += 1

            if idx >= 3000:
                break
        if idx >= 3000:
            break

    f.write("    );\n")
    f.write("};\n")

print("Fichier scene_3000_spheres.cfg généré : 3000 sphères OK")
