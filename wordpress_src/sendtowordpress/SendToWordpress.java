package sendtowordpress;

import java.awt.HeadlessException;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.Reader;
import java.net.MalformedURLException;
import java.util.Arrays;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import net.bican.wordpress.MediaObject;
import net.bican.wordpress.Page;
import net.bican.wordpress.Wordpress;
import redstone.xmlrpc.XmlRpcArray;
import redstone.xmlrpc.XmlRpcException;
import redstone.xmlrpc.XmlRpcFault;

public class SendToWordpress {
    static String user;
    static String password;
    static String xmlRpcUrl;
    static String titre;
    static String categories;
    static String tags;
    static String mainPict;
    static String otherPicts;
    static String htmlCode;
    static String excerpt;
    static String idRecipe;
    static boolean publier;

    public static void main(String[] args)
            throws IOException {
        if (args.length != 8) {
            System.out.println("ERREUR : nombre d'arguments incorrect (" + args.length + "). Veuillez rapporter le bug.");
            System.out.println("\"" + String.join("\", \"", args) + "\"");
        } else {
            xmlRpcUrl = args[0];
            categories = args[1];
            tags = args[2];
            mainPict = args[3];
            File htmlFile = new File(args[4]);
            String contentFile = loadFile(htmlFile);
            String[] parts = contentFile.split("\n");
            user = parts[0].replace("\r", "");
            password = parts[1].replace("\r", "");
            titre = parts[2].replace("\r", "");
            excerpt = parts[3].replace("\r", "");
            int i = 4;
            while (!parts[i].replace("\r", "").equals("<htmlCode>")) {
                excerpt = excerpt + "\n" + parts[i].replace("\r", "");
                i++;
            }
            htmlCode = parts[(i + 1)];
            for (int j = i + 2; j < parts.length; j++) {
                htmlCode = htmlCode + "\n" + parts[j];
            }
            otherPicts = args[5];
            publier = "true".equals(args[6]);
            idRecipe = args[7];
            if ((xmlRpcUrl != null) && (categories != null) && (mainPict != null) && (user != null) && (password != null) && (titre != null) && (excerpt != null) && (htmlCode != null) && (idRecipe != null)) {
                sendRecipe();
            } else {
                System.out.println("ERREUR : Le fichier envoyé est incorrect. Veuillez rapporter le bug.");
            }
        }
    }

    private static void sendRecipe() {
        try {
            Wordpress wp = new Wordpress(user, password, xmlRpcUrl);
            Page post = new Page();
            post.setTitle(titre);
            XmlRpcArray cats = new XmlRpcArray();
            if (!"null".equals(tags)) {
                post.setMt_keywords(tags);
            }
            List<String> catsRead = Arrays.asList(categories.split("\\|"));
            cats.addAll(catsRead);
            post.setCategories(cats);
            post.setDescription(htmlCode);
            post.setExcerpt(excerpt);
            String post_id;
            if (idRecipe.equals("-1")) {
                post_id = wp.newPost(post, publier);
            } else {
                post_id = idRecipe;
            }
            if (!mainPict.startsWith("http")) {
                MediaObject mediaImg = wp.newMediaObject("image/jpg", new File(mainPict), Boolean.FALSE, post_id);
                post.setThumbnail(mediaImg.getId());
            }
            String publish_statut = publier ? "1" : "0";
            wp.editPost(Integer.valueOf(post_id), post, publish_statut);
            if (!"null".equals(otherPicts)) {
                String[] oPicts = otherPicts.split("\\|");
                int j = oPicts.length;
                for (String otherImage : oPicts) {
                    String ext = otherImage.substring(otherImage.lastIndexOf(".")).toLowerCase();
                    if (!ext.equals(".png")) {
                        wp.newMediaObject("image/jpg", new File(otherImage), Boolean.FALSE, post_id);
                    } else {
                        wp.newMediaObject("image/png", new File(otherImage), Boolean.FALSE, post_id);
                    }
                }
            }
            System.out.println("!!!Send recipe SUCCEEDED!!!");
            System.out.println(post_id);
        } catch (MalformedURLException | XmlRpcFault | XmlRpcException | NumberFormatException | HeadlessException ex) {
            System.out.println("ERROR : send recipe failed.");
            Logger.getLogger(SendToWordpress.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    private static void testVariables() {
        System.out.println("USER :");
        System.out.println(user);
        System.out.println("PASSWD :");
        System.out.println(password);
        System.out.println("PUBLISH ADRESS :");
        System.out.println(xmlRpcUrl);
        System.out.println("TITRE :");
        System.out.println(titre);
        System.out.println("CATEGORIES :");
        System.out.println(categories);
        System.out.println("TAGS :");
        System.out.println(tags);
        System.out.println("IMAGE PRINCIPALE :");
        System.out.println(mainPict);
        System.out.println("HTML CODE :");
        System.out.println(htmlCode);
        System.out.println("AUTRES IMAGES :");
        System.out.println(otherPicts);
        System.out.println("PUBLIER :");
        System.out.println(publier);
        System.out.println("EXCERPT :");
        System.out.println(excerpt);
    }

    public static String loadFile(File file)
            throws IOException {
        InputStream fileReader = new FileInputStream(file);

        Reader utfReader = new InputStreamReader(fileReader, "utf-8");

        BufferedReader input = new BufferedReader(utfReader);

        String separator = System.getProperty("line.separator");

        StringBuilder builder = new StringBuilder();
        String line;
        while ((line = input.readLine()) != null) {
            builder.append(line);
            builder.append(separator);
        }
        input.close();
        return builder.toString();
    }
}
