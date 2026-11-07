#include "views/LegalText.h"

#include <iterator>

namespace installer::legal
{
    const LegalSection kTerms[] = {
        {"1. Identificación del titular",
         "Este software es ofrecido por [RAZÓN SOCIAL O NOMBRE DEL TITULAR], identificado con NIT [NIT], con domicilio en [CIUDAD, PAÍS] y canal de contacto [CORREO O MEDIO DE CONTACTO], en adelante, el titular."},
        {"2. Objeto y alcance",
         "Estos términos establecen las condiciones generales para la instalación y utilización del sistema de punto de venta Miscelánea Bellavista POS. La versión final debe describir los módulos, funcionalidades y modalidades de licencia que efectivamente se entregan al usuario."},
        {"3. Uso autorizado",
         "El usuario se compromete a utilizar el software para fines lícitos y de acuerdo con la documentación, permisos y alcance de la licencia adquirida. No debe intentar acceder sin autorización a sistemas, cuentas o datos de terceros, ni alterar el funcionamiento del producto."},
        {"4. Cuentas y credenciales",
         "El usuario es responsable de administrar adecuadamente las cuentas que cree, proteger sus credenciales y limitar el acceso al personal autorizado. El titular debe completar aquí las reglas reales de administración, recuperación y suspensión de cuentas."},
        {"5. Información y copias de seguridad",
         "El usuario debe definir y mantener una política de copias de seguridad adecuada para la información de su negocio. La versión definitiva debe precisar qué herramientas de respaldo ofrece el software, si las ofrece, y las responsabilidades de cada parte."},
        {"6. Actualizaciones y soporte",
         "El titular debe indicar el alcance del soporte, los canales de atención, los horarios, las condiciones de mantenimiento y la forma en que se comunican e instalan las actualizaciones."},
        {"7. Propiedad intelectual y licencia",
         "La versión definitiva debe identificar al titular de los derechos sobre el software, describir la licencia concedida, sus límites, vigencia, restricciones de copia o distribución y las condiciones de terminación."},
        {"8. Disponibilidad y responsabilidad",
         "El titular debe describir de forma clara las garantías, limitaciones y responsabilidades aplicables al producto, respetando las normas imperativas de la jurisdicción correspondiente. No deben incluirse exclusiones que contradigan derechos irrenunciables del consumidor."},
        {"9. Modificaciones de estos términos",
         "Indica cómo se notificarán los cambios, desde qué fecha aplicarán y qué opciones tendrá el usuario cuando una modificación requiera una nueva aceptacion."},
        {"10. Legislación y contacto",
         "Jurisdicción aplicable: [PAÍS Y NORMATIVA]. Para consultas sobre estos términos, el usuario podrá comunicarse mediante [CORREO O CANAL OFICIAL]."},
    };

    const LegalSection kPrivacy[] = {
        {"1. Responsable del tratamiento",
         "Responsable: [RAZÓN SOCIAL O NOMBRE]. NIT: [NIT]. Dirección: [DIRECCIÓN]. Correo para consultas y solicitudes: [CORREO DE PRIVACIDAD]. Estos datos deben corresponder al responsable real del tratamiento."},
        {"2. Datos que pueden tratarse",
         "Completa esta sección según el funcionamiento real del sistema. Por ejemplo, puede involucrar datos de identificación y contacto de usuarios, información de cuentas y registros operativos o comerciales ingresados por el negocio. No incluyas categorias que el software no recolecte ni procese."},
        {"3. Finalidades del tratamiento",
         "Describe las finalidades concretas: autenticación y administración de usuarios, operación de los módulos contratados, gestión de ventas e inventario, soporte técnico, seguridad y cumplimiento de obligaciones legales, únicamente cuando esas actividades correspondan a la implementación real."},
        {"4. Base legal y autorización",
         "Indica la base jurídica aplicable a cada finalidad y el mecanismo mediante el cual se obtiene la autorización cuando esta sea necesaria. En Colombia, esta sección debe ajustarse a la Ley 1581 de 2012, sus normas reglamentarias y las instrucciones vigentes de la autoridad competente."},
        {"5. Encargados y destinatarios",
         "Identifica proveedores, encargados o terceros que puedan acceder a los datos, el servicio que prestan, las condiciones de transmisión o transferencia y las garantías aplicables. Si no existen terceros, describe el escenario real."},
        {"6. Conservacion y eliminacion",
         "Define los periodos o criterios de conservacion, las reglas de eliminacion, los respaldos y el tratamiento de la información cuando termina la relación con el usuario o deja de ser necesaria."},
        {"7. Derechos de los titulares",
         "Describe cómo los titulares pueden conocer, actualizar y rectificar sus datos, solicitar prueba de la autorización, conocer el uso dado a la información, presentar consultas o reclamos y ejercer los demas derechos reconocidos por la normativa aplicable."},
        {"8. Seguridad de la información",
         "Describe únicamente las medidas técnicas, humanas y administrativas que realmente se implementan para proteger la información. Evita prometer cifrado, anonimización, copias de seguridad u otras medidas si no están implementadas y verificadas."},
        {"9. Cambios en la política",
         "Explica cómo se publicarán las modificaciones, cómo se informarán a los titulares y desde qué fecha entrarán en vigencia."},
        {"10. Consultas y reclamos",
         "Canal de atención: [CORREO O DIRECCIÓN]. Responsable de atender solicitudes: [AREA O PERSONA RESPONSABLE]. Procedimiento y tiempos de respuesta: [PROCEDIMIENTO APLICABLE]."},
    };

    LegalDocument termsDocument()
    {
        return {"TÉRMINOS Y CONDICIONES", kTerms, std::size(kTerms)};
    }

    LegalDocument privacyDocument()
    {
        return {"POLÍTICA DE PRIVACIDAD Y TRATAMIENTO DE DATOS", kPrivacy, std::size(kPrivacy)};
    }
}
