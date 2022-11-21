<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:diff="http://www.via.ecp.fr/~remi/soft/xml/xmldiff" >

<xsl:variable name="filter">*[@diff:status='removed']|*[count(descendant::*[@diff:status='removed']) &gt; 0]</xsl:variable>

<xsl:include href="helper-default-filter.xsl" />

</xsl:stylesheet>